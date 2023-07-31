#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stoi;
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

    linestream >> os >> kernel >> version;
  }
  return version;
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
        pids.emplace_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}
// TODO: Read and return the system memory utilization
  // ? Calculation : (Total memory - free memory) / Total Memory
float LinuxParser::MemoryUtilization() { 
  string line, label, value;
  float total, free;
  
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
 	  std::istringstream linestream(line);
      while (linestream >> label >> value) {
        if (label == "MemTotal:") {
          total = stof(value);
        }
		if (label == "MemFree:") {
          free = stof(value);
        }
      }
    }
  }
  return (total - free) / total;
}
//use kernel example
// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
   	std::istringstream linestream(line);
    linestream >> uptime;
  }
  //stol--> converts string to long int
  return std::stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
}

// Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  string line, value;
  vector<string> values {};
  long total;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
 	    std::istringstream linestream(line);
      while (linestream >> value) {
        values.emplace_back(value);
      }
    }
  }
  total = stoi(values[13]) + stoi(values[14]) + stoi(values[15]) + stoi(values[16]);  // total=utime+stime+cutime+cstime per stack overflow
  return total / sysconf(_SC_CLK_TCK);
}

long LinuxParser::ActiveJiffies() {
  // Read and return the number of active jiffies for the system
  auto jiffies = CpuUtilization();

  return stol(jiffies[CPUStates::kUser_]) + stol(jiffies[CPUStates::kNice_]) +
         stol(jiffies[CPUStates::kSystem_]) + stol(jiffies[CPUStates::kIRQ_]) +
         stol(jiffies[CPUStates::kSoftIRQ_]) +
         stol(jiffies[CPUStates::kSteal_]);
}

long LinuxParser::IdleJiffies() {
  // Read and return the number of idle jiffies for the system
  auto jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kIdle_]) + stol(jiffies[CPUStates::kIOwait_]);
}


vector<string> LinuxParser::CpuUtilization() {
  // Read and return CPU utilization
  string line, cpu, value;
  vector<string> jiffies;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);

    linestream >> cpu;

    while (linestream >> value) {
      jiffies.emplace_back(value);
    }
  }
  return jiffies;
}

int LinuxParser::TotalProcesses() {
  // Read and return the total number of processes
  int processes;
  string key, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "processes") {
        linestream >> processes;
        break;
      }
    }
  }
  return processes;
}

int LinuxParser::RunningProcesses() {
  // Read and return the number of running processes
  int processes;
  string key, line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "procs_running") {
        linestream >> processes;
        break;
      }
    }
  }
  return processes;
}


// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string command;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, command);
  }
  return command;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line, label, value;				 		
  long ram;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
 	  std::istringstream linestream(line);
      while (linestream >> label >> value) {
        if (label == "VmRSS:") {
          ram = stol(value) / 1000;// convert kilobyte value to megabytes
        }
      }
    }
  }
  return to_string(ram); // covert intram into a string
  ;
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line, label, value, user;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
 	  std::istringstream linestream(line);
      while (linestream >> label >> value) {
        if (label == "Uid:") {
          user = value;
        }
      }
    }
  }
  return user;
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line, label, value, id_value, user;
  string uid = Uid(pid);
  
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
 	  std::istringstream linestream(line);
      while (linestream >> label >> value >> id_value) {
        if (id_value == uid) {//this matches the balue to the uid
          user = label;//gives you the name of the user
        }
      }
    }
  }
  return user;
}


// DONE: Read and return the uptime of a process


long LinuxParser::UpTime(int pid) {
  string line, value;
  vector<string> ticks {};
  int upTimePid;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
 	    std::istringstream linestream(line);
      while (linestream >> value) {
        ticks.emplace_back(value);
      }
    }
  }
  upTimePid = UpTime() - stol(ticks[21]) / sysconf(_SC_CLK_TCK);	// return the starttime (in seconds) per stack overflow
  return upTimePid;
}