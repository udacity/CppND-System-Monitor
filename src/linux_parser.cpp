#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
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
int LinuxParser::TotalProcesses() { return Pids().size(); }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  int running = 0;
  string line, temp, status;
  for (auto pid : Pids()) {
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
      while (line.find("State:") ==  std::string::npos) {
        std::getline(stream, line);
      }
      std::istringstream linestream(line);
      linestream >> temp >> status;
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
  string ram;
  string temp;
  string line;

  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (line.find("VmSize:") ==  std::string::npos) {
      std::getline(stream, line);
    }
    std::istringstream linestream(line);
    linestream >> temp >> ram;
    ram_mb = stoi(ram) / 1000;
  }
  
  return to_string(ram_mb);

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
