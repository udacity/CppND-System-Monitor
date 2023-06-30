#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

#include "linux_parser.h"

std::string LinuxParser::OperatingSystem()
{
  std::string line;
  std::string key;
  std::string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

std::string LinuxParser::Kernel()
{
  std::string os, kernel, version;
  std::string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
std::vector<int> LinuxParser::Pids()
{
  std::vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      std::string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization()
{
  std::string line;
  float mem_total, mem_available;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::string mem_name;
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> mem_name >> mem_total;
    std::getline(stream, line);
    std::getline(stream, line);
    std::istringstream linestream2(line);
    linestream2 >> mem_name >> mem_available;
  }
  return (mem_total - mem_available) / mem_total;
}

long LinuxParser::UpTime()
{
  long uptime;
  std::string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  }
  return uptime;
}

long LinuxParser::Jiffies()
{
  return ActiveJiffies() + IdleJiffies();
}

long LinuxParser::ActiveJiffies(int pid)
{
  std::string line;
  std::string line_element;
  long utime;
  long stime;
  long cutime;
  long cstime;
  std::ifstream stream(kProcDirectory + '/' + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    // We don't care about the first 12 elements, so stream those away
    for (int i = 0; i < 13; ++i) {
      linestream >> line_element;
    }
    linestream >> utime; // 13th element (0 indexed)
    linestream >> stime; // 14th element (0 indexed)
    linestream >> cutime; // 15th element (0 indexed)
    linestream >> cstime; // 16th element (0 indexed)
  }
  return utime + stime + cutime + cstime;
}

long LinuxParser::ActiveJiffies()
{
  std::vector<long> cpu_util_vec = CpuUtilization();
  return (
    cpu_util_vec[CPUStates::kUser_] + cpu_util_vec[CPUStates::kNice_] +
    cpu_util_vec[CPUStates::kSystem_] + cpu_util_vec[CPUStates::kIRQ_] +
    cpu_util_vec[CPUStates::kSoftIRQ_] + cpu_util_vec[CPUStates::kSteal_]);
}

long LinuxParser::IdleJiffies()
{
  std::vector<long> cpu_util_vec = CpuUtilization();
  return cpu_util_vec[CPUStates::kIdle_] + cpu_util_vec[CPUStates::kIOwait_];
}

std::vector<long> LinuxParser::CpuUtilization()
{
  std::vector<long> cpu_utilization;
  long cpu_element;
  std::string key_name;
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> key_name;
    while (linestream >> cpu_element) {
      cpu_utilization.push_back(cpu_element);
    }
  }
  return cpu_utilization;
}

int LinuxParser::TotalProcesses()
{
  int total_processes;
  std::string identifier;
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> identifier;
      if (identifier == "processes") {
        linestream >> total_processes;
        break;
      }
    }
  }
  return total_processes;
}

int LinuxParser::RunningProcesses()
{
  int running_processes;
  std::string identifier;
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> identifier;
      if (identifier == "procs_running") {
        linestream >> running_processes;
        break;
      }
    }
  }
  return running_processes;
}

std::string LinuxParser::Command(int pid)
{
  std::string cmd_string;
  std::ifstream stream(kProcDirectory + '/' + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmd_string);
  }
  return cmd_string;
}

std::string LinuxParser::Ram(int pid)
{
  int ram = 0;
  std::string identifier;
  std::string line;
  std::ifstream stream(kProcDirectory + '/' + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> identifier;
      if (identifier == "VmSize:") {
        linestream >> ram;
        break;
      }
    }
  }
  return std::to_string(ram / 1000);
}

std::string LinuxParser::Uid(int pid)
{
  std::string uid;
  std::string identifier;
  std::string line;
  std::ifstream stream(kProcDirectory + '/' + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> identifier;
      if (identifier == "Uid:") {
        linestream >> uid;
        break;
      }
    }
  }
  return uid;
}

std::string LinuxParser::User(int pid)
{
  std::string p_uid = Uid(pid);
  std::string user;
  std::string x;
  std::string user_uid;
  std::string line;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> user_uid;
      if (user_uid == p_uid) {
        break;
      }
    }
  }
  return user;
}

long LinuxParser::UpTime(int pid)
{
  std::string line;
  std::string line_element;
  long start_time;
  std::ifstream stream(kProcDirectory + '/' + std::to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    // We don't care about the first 20 elements, so stream those away
    for (int i = 0; i < 21; ++i) {
      linestream >> line_element;
    }
    // The 21st element (0 indexed) is the process starttime
    linestream >> start_time;
  }

  // Get system uptime
  long sys_uptime = UpTime();
  return sys_uptime - (start_time / sysconf(_SC_CLK_TCK));
}
