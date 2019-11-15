#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

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

string LinuxParser::Kernel() {
  string os, label, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> label >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

float LinuxParser::MemoryUtilization() {
  string mem_total_key = "MemTotal:";
  string mem_free_key = "MemFree:";
  float mem_total = 0.0;
  float mem_free = 0.0;

  string line, key, value;

  std::ifstream filestream(kProcDirectory + kMeminfoFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> value) {
        if (key == mem_total_key) {
          mem_total = stof(value);
        }
        if (key == mem_free_key) {
          mem_free = stof(value);
          break;
        }
      }
    }
  }
  return (mem_total - mem_free)/mem_total;
}

long int LinuxParser::UpTime() {
  string uptime, line;
  long int value;
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
    value = stol(uptime);
  }
  return value;
}

long LinuxParser::Jiffies() {
 return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

long LinuxParser::ActiveJiffies(int pid) {
  string line, key, user, nice, system, idle, iowait, irq, softirq, steal;
  int long value;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal) {
        if (key == "cpu") {
          value = stol(user) + stol(nice) + stol(system) + stol(irq) + stol(softirq) + stol(steal);
        }
      }
    }
  }

  return value;
}

long LinuxParser::ActiveJiffies() {
  string line, key, user, nice, system, idle, iowait, irq, softirq, steal;
  int long value;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal) {
        if (key == "cpu") {
           value = stol(user) + stol(nice) + stol(system) + stol(irq) + stol(softirq) + stol(steal);
        }
      }
    }
  }

  return value;
}

long LinuxParser::IdleJiffies() {
  string line, key, user, nice, system, idle, iowait, irq, softirq, steal;
  int long value;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> user >> nice >> system >> idle) {
        if (key == "cpu") {
          value = stol(idle);
        }
      }
    }
  }

  return value;
}

float LinuxParser::CpuUtilization() {
  float active = LinuxParser::ActiveJiffies();
  float total = LinuxParser::Jiffies();

  return active/total;
}

float LinuxParser::CpuUtilization(int pid) {
  string line, v;
  float utime, stime, cutime, cstime, starttime;
  int uptime = LinuxParser::UpTime();
  int utime_idx = 14;
  int stime_idx = 15;
  int cutime_idx = 16;
  int cstime_idx= 17;
  int starttime_idx = 22;

  int counter = 0;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while ((linestream >> v) && (counter <= starttime_idx)) {
        counter++;
        if (counter == utime_idx)   { utime   = stof(v); }
        if (counter == stime_idx)   { stime   = stof(v); }
        if (counter == cutime_idx)  { cutime  = stof(v); }
        if (counter == cstime_idx)  { cstime  = stof(v); }
        if (counter == starttime_idx)  { starttime  = stof(v); }
      }
    }
  }

  int Hertz = sysconf(_SC_CLK_TCK);
  float total_time = utime + stime + cutime + cstime;
  float seconds = uptime - (starttime / Hertz);
  float cpu_usage = 100 * ((total_time / Hertz) / seconds);

  return cpu_usage;
}

int LinuxParser::TotalProcesses() {
  string line, key, count;
  int value = 0;

  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> count) {
        if (key == "processes") {
          value = stoi(count);
        }
      }
    }
  }
  return value;
}

int LinuxParser::RunningProcesses() {
  string line;
  string key;
  string value;
  int proc_count = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> value) {

        if (key == "procs_running") {
          proc_count = stoi(value);
        }
      }
    }
  }

  return proc_count;
}

string LinuxParser::Command(int pid) {
  string line, comm;
  std::ifstream stream(kProcDirectory + to_string(pid) + "/comm");

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> comm;
}

  return line;
}

string LinuxParser::Ram(int pid) {
  string line, key, kbs;
  string ram = "0.0";

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> kbs) {
        if (key == "VmSize:") {
          ram = to_string(stoi(kbs)/1000);
        }
      }
    }
  }
  return ram;
}

string LinuxParser::Uid(int pid) {
  string line, key, uid, value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while (linestream >> key >> uid) {
        if (key == "Uid:") {
          value = uid;
        }
      }
    }
  }
  return value;
}

string LinuxParser::User(int pid) {
  string line, user, x, uid, num, value;
  std::ifstream filestream(kPasswordPath);

  uid = LinuxParser::Uid(pid);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      while (linestream >> user >> x >> num) {
        if (num == uid) {
          value = user;
        }
      }
    }
  }
  return value;
}

long LinuxParser::UpTime(int pid) {
  string line, key;
  int counter = 0;
  int uptime_idx = 22;
  int long value;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);

  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);

      while ((linestream >> key) && (counter < uptime_idx)) {
        counter++;

        if (counter == uptime_idx) {
           value = stol(key)/sysconf(_SC_CLK_TCK);
        }
      }
    }
  }

  return value;
}
