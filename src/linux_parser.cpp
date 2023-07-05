#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "../include/linux_parser.h"

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
  string line;
  string key;
  float value;
  string value_units;
  float mem_total = 0;
  float mem_free = 0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> value_units) {
        if (key == "MemTotal" && value_units == "kB")
          mem_total = value;
        else if (key == "MemFree" && value_units == "kB")
          mem_free = value;
      }
    }
  }
  return (mem_total - mem_free) / mem_total;
  }

// Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  double uptime = 0;
  double uptime_incl_suspend = 0;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> uptime >> uptime_incl_suspend;
  }
  return static_cast<long int>(uptime);
  }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  std::string line;
  std::string key;
  std::string cpu_value{};
  std::vector<string> cpu_values{};

  // Access file, read, and store stat data
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key) {
        if (key == "cpu") {
          while (linestream >> cpu_value) {
            cpu_values.push_back(cpu_value);
          }
          return cpu_values;
        }
      }
    }
  }
  return cpu_values;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  string key;
  int value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  string key;
  int value = 0;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return value;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  string cmd = "";

  // Get UID
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> cmd;
  }

  return cmd;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key = "";
  int ram = 0;
  int mb_per_kb = 1024;

  // Get UID
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && key != "VmSize") {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> ram) {
        if (key == "VmSize")
          break;
      }
    }
  }

  return to_string( ram / mb_per_kb );
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key = "";
  string uid = "";

  // Get UID
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && key != "Uid") {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> uid) {
        if (key == "Uid")
          break;
      }
    }
  }

  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string line;
  string uid = "";
  string username = "";
  string pass_col1 = "";
  string pass_uid = "";

  // Get Username
  uid = Uid(pid);
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line) && pass_uid != uid) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> username >> pass_col1 >> pass_uid) {
        if (pass_uid == uid)
          break;
      }
    }
  }

  return username;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line;
  string key;
  string value = "";
  long uptime = 0;
  int index = 22;  // 22nd item is updtime in ticks
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for ( int i = 0; i < index - 1; i++ ) {
      linestream >> value;
      }
    linestream >> uptime;  // Uptime in ticks.
  }

  return uptime / sysconf(_SC_CLK_TCK);  // Convert ticks to seconds.
}

// Read and return CPU utilization of a process
vector<string> LinuxParser::CpuUtilization(int pid) {
  std::string line;
  std::string key;
  std::string stat{};
  std::vector<string> stat_values{};

  // Access file, read, and store stat data
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    while (linestream >> stat) {
        stat_values.push_back(stat);
      }
  }
  return stat_values;
}
