#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

#include <filesystem>
using std::filesystem::directory_iterator;
using std::filesystem::is_directory;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// Updated to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  
  for (const auto& file : directory_iterator(kProcDirectory)) {
    if (is_directory(file.status())) {
      string filename = file.path().filename();
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  
  return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string temp, memTotal, memFree, memAvailable;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if (stream.is_open()) {
    //Read total memory
    while (line.find("Total:") ==  std::string::npos) {
      std::getline(stream, line);
    }
    std::istringstream linestream(line);
    linestream >> temp >> memTotal;

    // Read free memory
    while (line.find("Free:") ==  std::string::npos) {
      std::getline(stream, line);
    }
    std::istringstream linestream2(line);
    linestream2 >> temp >> memFree;

    // Read available memory
    while (line.find("Available:") ==  std::string::npos) {
      std::getline(stream, line);
    }
    std::istringstream linestream3(line);
    linestream3 >> temp >> memAvailable;

  }
  float memUtilization = stof(memTotal) - stof(memFree) - stof(memAvailable);
 
  return memUtilization / stof(memTotal); 
}

// Read and return the system uptime
long LinuxParser::UpTime() { 
  string upTime, idleTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  
  return stol(upTime);
 }

// Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  vector<string> cpu_states = CpuUtilization();
  long totalJiffies = 0;
  
  for (auto state : cpu_states) {
    totalJiffies += stol(state);
  }
  return totalJiffies;
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  string utime, stime, cutime, cstime, starttime;
  string temp, line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    // go through first 13 values
    for (int i = 0; i < 13; i++) {
      linestream >> temp;
    }

    // read next 4
    linestream >> utime >> stime >> cutime >> cstime;

    // go through next 4 values
    for (int i = 0; i < 4; i++) {
      linestream >> temp;
    }
    // read last value
    linestream >> starttime;
  }
  long activeJiffies = stol(utime) + stol(stime) + stol(cutime) + stol(cstime);
  return activeJiffies; 
}

// Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  vector<string> cpu_states = CpuUtilization();
  // all except idle and iowait counted as active
  // return stol(cpu_states[kUser_]) + stol(cpu_states[kNice_]) +
  //        stol(cpu_states[kSystem_]) + stol(cpu_states[kIRQ_]) + 
  //        stol(cpu_states[kSoftIRQ_]) + stol(cpu_states[kSteal_]) + 
  //        stol(cpu_states[kGuest_]) + stol(cpu_states[kNice_]);
  return Jiffies() - IdleJiffies();
}

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> cpu_states = CpuUtilization();
 
  // idle and iowait counted as idle
  return stol(cpu_states[kIdle_]) + stol(cpu_states[kIOwait_]);
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> cpu_states;
  string temp;
  string line;

  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    // read and discard cpu_label
    linestream >> temp;
    // read remaining states
    for (int i = 0; i != 9; i++) {
      linestream >> temp;
      cpu_states.push_back(temp);
    }
  }
  return cpu_states;
 }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { return Pids().size(); }

// Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int running = 0;
  string line, temp, status;
  for (auto pid : Pids()) {
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    if (stream.is_open()) {
      std::istringstream linestream(line);
      linestream >> temp >> temp >> status;
      if (status == "R") {
        running++;
      }
    }
  }
  return running;
 }

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmdline;
  string line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cmdline;
  }
  return cmdline;   
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  int ram_mb;
  string ram = "N/A";
  string temp;
  string line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      if (line.find("VmSize:") != std::string::npos) {
        std::istringstream linestream(line);
        linestream >> temp >> ram;
        ram_mb = stoi(ram) / 1000;
        ram = to_string(ram_mb);
        return ram;
      }
    }  
  }
  return ram;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string uid;
  string line, temp;
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (line.find("Uid:") ==  std::string::npos) {
      std::getline(stream, line);
    }
    std::istringstream linestream(line);
    linestream >> temp >> uid;
  }
  
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string username;
  string uid = Uid(pid); // get uid from pid
  string line;

  std::ifstream stream(kPasswordPath);

  if (stream.is_open()) {
    // search for uid
    while (line.find(":" + uid) ==  std::string::npos) {
      std::getline(stream, line);
    }
    std::istringstream linestream(line);
    linestream >> username;
    int index = username.find(":");
    username = username.substr(0, index);
  }
  return username;
}

// Read and return the uptime of a process
// TODO - fix errors
long LinuxParser::UpTime(int pid) {string username;
  string utime_ticks;
  string temp;
  string line;
  string uid = Uid(pid); // get uid from pid

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; i++) {
       linestream >> temp;
    }
    linestream >> utime_ticks;
  }
  long utime_seconds = stol(utime_ticks) / sysconf(_SC_CLK_TCK);
  return utime_seconds;
}
