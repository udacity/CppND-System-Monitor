#include <bits/stdc++.h>
#include <dirent.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::atoi; // convert string to int
using std::stof; // convert to float
using std::atol; // convert to long
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string MemTotal, MemFree, MemAvailable, Buffers, title;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> title >> MemTotal;

    std::getline(stream, line);
    std::istringstream linestreamMemFree(line);
    linestreamMemFree >> title >> MemFree;

    std::getline(stream, line);
    std::istringstream linestreamMemAvailable(line);
    linestreamMemAvailable >> title >> MemAvailable;

    std::getline(stream, line);
    std::istringstream linestreamBuffers(line);
    linestreamBuffers >> title >> Buffers;
  }

  float memUtil =
      (stof(MemTotal.c_str()) - stof(MemFree.c_str())) / stof(MemTotal.c_str());
  return memUtil;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string title, upTime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime;
  }
  return atol(upTime.c_str());
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> jiffies = CpuUtilization();
  long total_jiffies_since_boot = 0;
  for(string s : jiffies){
    total_jiffies_since_boot += atol(s.c_str());
  }
  return total_jiffies_since_boot; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  vector<string> jiffies = CpuUtilization();
  // idle + iowait
  long idle_jiffies_since_boot = 
    atol(jiffies[3].c_str()) ;
  
  return idle_jiffies_since_boot; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<string> v{};
  string user, nice, system, idle, iowait, irq, softirq, steal;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
  }
  v.push_back(user);
  v.push_back(nice);
  v.push_back(system);
  v.push_back(idle);
  v.push_back(iowait);
  v.push_back(irq);
  v.push_back(softirq);
  v.push_back(steal);

  return v; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string name = "processes";
  string title, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  while(std::getline(stream, line)){
    if(line.compare(0, name.size(), name) == 0){
      std::istringstream linestream(line);
      linestream >> title >> value;
      break;
    }
  }
  return atoi(value.c_str()); 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string name = "procs_running";
  string title, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  string line;
  while(std::getline(stream, line)){
    if(line.compare(0, name.size(), name) == 0){
      std::istringstream linestream(line);
      linestream >> title >> value;
      break;
    }
  }
  return atoi(value.c_str()); 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
  string cmd;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, cmd);
  }
  return cmd; 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string name = "Uid";
  string title, value;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  string line;
  while(std::getline(stream, line)){
    if(line.compare(0, name.size(), name) == 0){
      std::istringstream linestream(line);
      linestream >> title >> value;
      break;
    }
  }
  return value; 
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) {
 return 0;
}