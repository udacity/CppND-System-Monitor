#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <sstream>
#include <string>
#include <vector>

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
  string os, kernel, version;
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
// Standard linux gcc doesn't support std::filesystem on ubuntu yet (introduced
// in c++17)
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

// DONE: Read and return the system memory utilization; returns a float that is the percentage of memory used
float LinuxParser::MemoryUtilization() { 
  float mem_total, mem_free;
  string line, key, value, units;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value >> units) {
        if (key == "MemTotal:"){ mem_total = std::stof(value);}
        else if (key == "MemFree:"){ mem_free = std::stof(value);}
      }
    }
  }
  return (mem_total - mem_free)/mem_total; 
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  long uptime, idle_time;
  std::ifstream stream(kProcDirectory + kUptimeFilename);  
  if (stream.is_open()){
    stream >> uptime;
    stream >> idle_time;
  }
  return uptime; 
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  long total = 0;
  for(std::string s : LinuxParser::CpuUtilization()){
    total += std::stol(s);
  }  
  return total;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid [[maybe_unused]]) { return 0; }

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  long active = 0;
  vector<std::string> s = LinuxParser::CpuUtilization();
  active += std::stol(s[0]);
  active += std::stol(s[1]);
  return active;
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  long idle = 0;
  vector<std::string> s = LinuxParser::CpuUtilization();
  idle += std::stol(s[3]);
  idle += std::stol(s[4]);
  return idle;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  vector<std::string> CPU_Util; 
  std::string line, key, user, nice, sys, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open() && std::getline(stream, line)){
    std::istringstream linestream(line);
    //std::cout<<"LINE: "<<line<<"\n";
    linestream >> key >> user >> nice >> sys >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    CPU_Util.push_back(user);
    CPU_Util.push_back(nice);
    CPU_Util.push_back(sys);
    CPU_Util.push_back(idle);
    CPU_Util.push_back(iowait);
    CPU_Util.push_back(irq);
    CPU_Util.push_back(softirq);
    CPU_Util.push_back(steal);
    CPU_Util.push_back(guest);
    CPU_Util.push_back(guest_nice);
  }
  return CPU_Util;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes"){ return std::stoi(value);}
      }
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line, key, value;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running"){ return std::stoi(value);}
      }
    }
  }
  return 0;
 }

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string line = "";
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);
  if(stream.is_open()){
    std::getline(stream, line);
    return line;
  }
  return line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, key, value;
  value = "";
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "VmSize"){ return value; }
      }
    }
  }
  return value;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, value;
  value = "";
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> value){
        if (key == "Uid"){ return value; }
      }
    }
  }
  return value;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) { 
  string uid = Uid(pid);
  string line, key, other;
  string value = "";
  std::ifstream stream(kPasswordPath);
  if(stream.is_open()){
    while(std::getline(stream, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> value >> other >> key){
        if(key == uid){ return value;}
      }
    }
  }
  return value;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid [[maybe_unused]]) { return 0; }
