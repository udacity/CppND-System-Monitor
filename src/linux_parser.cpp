#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::stol;
using std::string;
using std::to_string;
using std::vector;

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
      std::istringstream basicInputStringStream(line);
      while (basicInputStringStream >> key >> value) {
        if (key == kPrettyName) {
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
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream basicInputStringStream(line);
    basicInputStringStream >> os >> version >> kernel;
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
    // https://www.thegeekdiary.com/understanding-proc-meminfo-file-analyzing-memory-utilization-in-linux/
    // https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290
    // Memory Utilization = MemTotal - MemFree / MemTotal; MemFree = LowFree + HighFree
    string line, key, value;
    float memFree = 1.0f, memTotal = 1.0f; // avoid division by zero
    bool foundMemFree = false, foundMemTotal = false;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line)) {
            std::remove(line.begin(), line.end(), ' ');
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream basicInputStringStream(line);
            while (basicInputStringStream >> key >> value && !(foundMemFree && foundMemTotal)) {
                if (key == kMemTotal) {
                    memTotal = std::stof(value);
                    foundMemTotal = true;
                }
                else if (key == kMemFree) {
                    memFree = std::stof(value);
                    foundMemFree = true;
                }
            }
        }
    }
    return (memTotal - memFree) / memTotal;
}

// Read and return the system uptime
long LinuxParser::UpTime() {
    string line, value;
    long int uptime = 0;
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    if (stream.is_open()) {
        if (std::getline(stream, line)) {
            std::istringstream basicInputStringStream(line);
            basicInputStringStream >> value;
            uptime = std::stol(value);
        }
    }
    return uptime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
    string line, key, value;
    long int totalProcesses = 0;
    bool foundTotalProcesses = false;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line) && !foundTotalProcesses) {
            std::istringstream basicInputStringStream(line);
            while (basicInputStringStream >> key >> value) {
                if (key == kTotalProcesses) {
                    totalProcesses = std::stol(value);
                    foundTotalProcesses = true;
                    break;
                }
            }
        }
    }
    return totalProcesses;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
    string line, key, value;
    long int runningProcesses = 0;
    bool foundRunningProcesses = false;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while (std::getline(stream, line) && !foundRunningProcesses) {
            std::istringstream basicInputStringStream(line);
            while (basicInputStringStream >> key >> value) {
                if (key == kRunningProcesses) {
                    runningProcesses = std::stol(value);
                    foundRunningProcesses = true;
                    break;
                }
            }
        }
    }
    return runningProcesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
