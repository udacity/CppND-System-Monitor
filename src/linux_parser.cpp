#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>

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
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
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
  long int total, used, free = 0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          total = std::stoi(value);
        }
        if (key == "MemFree:") {
          free = std::stoi(value);
        }
      }
    }
  }

  used = total - free;
  return float(used) / float(total);
}

long LinuxParser::UpTime() {
  long uptime = 0;
  string uptimeString, idleString;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptimeString >> idleString;
  }

  uptime = std::stol(uptimeString, nullptr);

  return uptime;
}

long LinuxParser::Jiffies() { 
  string cpu, user, nice, system, idle, iowait, irq, softIrq, steal, guest, guestNice;
  int userInt, niceInt, systemInt, idleInt, iowaitInt, irqInt, softIrqInt, stealInt = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softIrq >> steal >> guest >> guestNice;

    userInt = std::stoi(user);
    niceInt = std::stoi(nice);
    systemInt = std::stoi(system);
    idleInt = std::stoi(idle);
    iowaitInt = std::stoi(iowait);
    irqInt = std::stoi(irq);
    softIrqInt = std::stoi(softIrq);
    stealInt = std::stoi(steal);
  }

  return userInt + niceInt + systemInt + idleInt + iowaitInt + irqInt + softIrqInt + stealInt;
}

float LinuxParser::CpuUtilization(int pid) {
  long hertz =  sysconf(_SC_CLK_TCK);
  
  string line, value;
  int utime, stime, cutime, cstime, starttime = 0;
  auto i = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line, ' ')) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        if (i == 13) {
          utime = std::stoi(value);
        }

        if (i == 14) {
          stime = std::stoi(value);
        }

        if (i == 15) {
          cutime = std::stoi(value);
        }

        if (i == 16) {
          cstime = std::stoi(value);
        }

        if (i == 21) {
          starttime = std::stoi(value);
        }
      }
      i++;
    }
  } else {
    return 0.0f;
  }
  
  long totalTime = utime + stime + cutime + cstime;
  long procUptime = UpTime(pid);
  float usage = (float(totalTime) / float(hertz)) / float(procUptime);

  return usage;
}

long LinuxParser::ActiveJiffies() {
  string cpu, user, nice, system, idle, iowait, irq, softIrq, steal, guest, guestNice;
  int userInt, niceInt, systemInt, irqInt, softIrqInt, stealInt = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softIrq >> steal >> guest >> guestNice;

    userInt = std::stoi(user);
    niceInt = std::stoi(nice);
    systemInt = std::stoi(system);
    irqInt = std::stoi(irq);
    softIrqInt = std::stoi(softIrq);
    stealInt = std::stoi(steal);
  }

  return userInt + niceInt + systemInt + irqInt + softIrqInt + stealInt;
}

long LinuxParser::IdleJiffies() {
  string cpu, user, nice, system, idle, iowait, irq, softIrq, steal, guest, guestNice;
  int idleInt, iowaitInt = 0;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softIrq >> steal >> guest >> guestNice;

    idleInt = std::stoi(idle);
    iowaitInt = std::stoi(iowait);
  }

  return idleInt + iowaitInt;
}

int LinuxParser::TotalProcesses() {
  int processes = 0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          processes = std::stoi(value);
          return processes;
        }
      }
    }
  }

  return -1;
}

int LinuxParser::RunningProcesses() {
  int processes = 0;
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          processes = std::stoi(value);
          return processes;
        }
      }
    }
  }

  return -1;
}

string LinuxParser::Command(int pid) { 
  string line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    return line;
  }
}

string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "VmSize:") {
          linestream >> value;
          return value;
        }
      }
    }
  }
}

string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "Uid:") {
          linestream >> value;
          return value;
        }
      }
    }
  }
}

string LinuxParser::User(int pid) {
  auto targetUid = Uid(pid);

  string line;
  string username, password, uid, gid, info, home, shell;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> password >> uid >> gid >> info >> home >> shell) {
        if (targetUid == uid) {
          return username;
        }
      }
    }
  }

  return "";
}

long LinuxParser::UpTime(int pid) {
  auto uptime = UpTime();

  string line;
  string value;
  auto i = 0;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line, ' ')) {
      std::istringstream linestream(line);
      while (linestream >> value) {
        if (i == 21) {
          auto ticks = std::stoi(value);
          auto upSinceBoot = ticks / sysconf(_SC_CLK_TCK);

          return uptime - upSinceBoot;
        }
      }
      i++;
    }
  }

  return 0;
}