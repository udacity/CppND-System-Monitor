#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "readFromFile.h"
#include "format.h"

using std::stof; // string to float
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
/*   string line;
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
  return value; */

    std::string path_OS = "/etc/os-release";
    std::string flag_OS = "PRETTY_NAME=";
    std::string string_OS = readFromFile(path_OS,flag_OS);
    return string_OS; 
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel; // pop tokens from stream
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
// Pid = process IDs = integer foldernames in /proc
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
  return pids; // return vector of process IDs
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 

    float memUse, memTot, memFree, memBuffer, memCache, value;
    std::ifstream fileStream(kProcDirectory + kMeminfoFilename);
    string line, key, unit;
    if (fileStream.is_open()) {
      while(std::getline(fileStream,line)){ // get each line from fileStream
        std::istringstream lineStream(line);
        lineStream >> key >> value >> unit; // key and value separated by spaces, unit not used
        if (key == "MemTotal:"){
          memTot = value;}
        if (key == "MemFree:"){
          memFree = value;}
        if (key == "Buffers:"){
          memBuffer = value;}
        if (key == "Cached:"){
          memCache = value;}
      }
    }
    memUse = (memTot - (memFree+memBuffer+memCache)) / memTot;
    return memUse;}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  float totProcesses, value;
    std::ifstream fileStream(kProcDirectory + kStatFilename);
    string line, key;
    if (fileStream.is_open()) {
      while(std::getline(fileStream,line)){ // get each line from fileStream
        std::istringstream lineStream(line);
        lineStream >> key >> value; // key and value separated by spaces
        if (key == "processes"){
          totProcesses = value;}
      }
    }
  return totProcesses; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  float runProcesses, value;
    std::ifstream fileStream(kProcDirectory + kStatFilename);
    string line, key;
    if (fileStream.is_open()) {
      while(std::getline(fileStream,line)){ // get each line from fileStream
        std::istringstream lineStream(line);
        lineStream >> key >> value; // key and value separated by spaces
        if (key == "procs_running"){
          runProcesses = value;}
      }
    }
  return runProcesses; }

// TODO: Read and return the system uptime 
long LinuxParser::UpTime() { 
  long timeUpSeconds, timeIdleSeconds, value;
    std::ifstream fileStream(kProcDirectory + kUptimeFilename);
    string line, key;
    if (fileStream.is_open()) {
      std::getline(fileStream, line); // get line from fileStream
      std::istringstream lineStream(line);
      lineStream >> timeUpSeconds >> timeIdleSeconds; 
      }
  return timeUpSeconds; }

// TODO: Read and return the number of jiffies for the system
// tick rate = clock frequency in Hz = declared in include/asm-i386/param.h
// Jiffies = number of ticks since system booted = declared in <linux/jiffies.h>
// seconds = (jiffies / Hz)
long LinuxParser::Jiffies() { 
  float runProcesses, value;
    std::ifstream fileStream(kProcDirectory + kStatFilename);
    string line, key;
    if (fileStream.is_open()) {
      while(std::getline(fileStream,line)){ // get each line from fileStream
        std::istringstream lineStream(line);
        lineStream >> key >> value; // key and value separated by spaces
        if (key == "procs_running"){
          runProcesses = value;}
      }
    }
  return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

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
