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
  string os,version, kernel;
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
  string line;
  string key;
  string value;
  float total,free,utilized;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename); //open the memory info file
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key=="MemTotal:") { //check for total memory value
          total = std::stof(value); //standard library function to convert string into float
        }
        if (key=="MemFree:"){ //check for free memory value
          free = std::stof(value);
        }
      }
    }
  }
  utilized= (total - free)/(total); //calculate the uitlization
  return utilized; 
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  string value;
  long uptime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename); // file containing uptime info
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  uptime = std::stol(value); //standard library function to convert string to long
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
vector<string> LinuxParser::CpuUtilization() { 
vector<string> utilization;
  string line;
  string key;
  string user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice; // the 10 ints beside the "cpu" in the /proc/stat file

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >>  guest_nice) {
        if (key == "cpu") {
          utilization = {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
          break;
        }
      }
    }
  }
  return utilization;
}  


// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string value;
  string key;
  int processes;
  std::ifstream filestream(kProcDirectory + kStatFilename); 
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          processes = std::stoi(value);
        }
      }
    }
  }
  return processes;
 }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string value;
  string key;
  int running_processes;
  std::ifstream filestream(kProcDirectory + kStatFilename); 
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          running_processes = std::stoi(value);
        }
      }
    }
  }
  return running_processes;
 }
// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> value;
  }
  return value;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  string line;
  string value;
  string key;
  string memory;
  float temp;
  string memoryMB;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename); 
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmData:") {
          memory = value;
          break;
        }
      }
    }
  }
  temp = std::stof(memory);
  temp /= 1024;
  memoryMB = std::to_string(temp);
  return memoryMB;
 }
  

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) { 
  string line;
  string value;
  string key;
  string uid;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename); 
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          uid = value;
          break;
        }
      }
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {  //this function should take the input as uid which can be taken from the above function
  string line;
  string key;
  string x;
  string value; 
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> x >> value) {
        if (std::stoi(value) == pid) {
          return key;
        }
      }
    }
  }
  return key;
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }