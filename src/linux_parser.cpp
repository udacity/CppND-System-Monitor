#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <iostream> //for debug
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

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
    float mem_total, mem_free, mem_usage;
    string key, value;
    string line;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    if (stream.is_open()) {
        while(std::getline(stream, line)) {
          std::istringstream linestream(line);
          while (linestream >> key >> value) {
              if (key == "MemTotal:") {
                  // std::cout<<"mem total"<<value<<std::endl;
                  mem_total = stof(value);
              }
              if (key == "MemFree:") {
                  // std::cout<<"mem free"<<value<<std::endl;
                  mem_free = stof(value);
              }
          }
        }
    }
    mem_usage = (mem_total - mem_free) / mem_total;
    mem_usage = round(10000 * mem_usage) / 10000;

    return mem_usage; 
}

// Done: Read and return the system uptime
long LinuxParser::UpTime() { 
    std::ifstream stream(kProcDirectory + kUptimeFilename);
    string line;
    string up_time;
    string idle_time;
    if (stream.is_open()) {
        while(std::getline(stream, line)) {
          std::istringstream linestream(line);
          linestream >> up_time >> idle_time;
          return std::stol(up_time);
        }
    }

    return 0;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
    long jiffies;
    vector<string> cpu_info = LinuxParser::CpuUtilization(); 
    for (auto i : cpu_info)   {
        jiffies += std::stoi(i); 
    }
    return jiffies;
}

// // TODO: Read and return the number of active jiffies for a PID
// // REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
    long total_time = 0;
    auto process_stat = LinuxParser::GetProcStatus(pid);
    //13 to 17
    long utime = std::stol(process_stat[13]);
    long stime = std::stol(process_stat[14]);
    long cutime = std::stol(process_stat[15]);
    long cstime = std::stol(process_stat[16]);

    //add children processes
    total_time = utime + stime + cutime + cstime; //ActiveJiffies
    return total_time; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    long active_jiffies;
    auto cpu_info = LinuxParser::CpuUtilization();
    long user = std::stoi(cpu_info[CPUStates::kUser_]);
    long nice = std::stoi(cpu_info[CPUStates::kNice_]);
    long irq = std::stoi(cpu_info[CPUStates::kIRQ_]);
    long sirq = std::stoi(cpu_info[CPUStates::kSoftIRQ_]);
    long system = std::stoi(cpu_info[CPUStates::kSystem_]);
    long steal = std::stoi(cpu_info[CPUStates::kSteal_]);
    active_jiffies = user + nice + irq + sirq + system + steal ;
    return active_jiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
    long idle_jiffies = 0;
    auto cpu_info = LinuxParser::CpuUtilization();

    long idle = std::stoi(cpu_info[CPUStates::kIdle_]);
    long iowait = std::stoi(cpu_info[CPUStates::kIOwait_]);
    idle_jiffies = idle + iowait;

    return idle_jiffies; 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
    vector<string> utils_info;

    string line;
    string cpu, buf;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
        while(std::getline(stream, line)) {
            std::istringstream linestream(line);
            linestream >> cpu;
            if (cpu == "cpu") {
                while (linestream >> buf) {
                    utils_info.emplace_back(buf);
                }
            }
        }
    }
    stream.close();
    return utils_info; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    int number_of_process = 0;
    string key, value;
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
      while(std::getline(stream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
            if (key == "processes") {
                number_of_process = stoi(value);
            }
        }
      }
    }
    stream.close();
    return number_of_process;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    int running_process = 0;
    string key, value;
    string line;
    std::ifstream stream(kProcDirectory + kStatFilename);
    if (stream.is_open()) {
      while(std::getline(stream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
            if (key == "procs_running") {
                running_process = stoi(value);
                stream.close();
                return running_process;
            }
        }
      }
    }

    return 0; 
}

// Done: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) { 
    std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
    string line;
    if (stream.is_open()) {
        std::getline(stream, line);
        if (!line.empty()) {
            stream.close();
            return line;
        }
            
    }
    return string(); 
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) { 
    string key, mem;
    string line;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
      while(std::getline(stream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> mem) {
            if (key == "VmSize:") {
                stream.close();
                return to_string(stol(mem) / 1024);   ;
            }
        }
      }
    } 
    return mem; 
}

// Done: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
    string key, uid;
    string line;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (stream.is_open()) {
      while(std::getline(stream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> uid) {
            if (key == "Uid:") {
                stream.close();
                return uid;
            }
        }
      }
    } 
    return uid; 
}

// Done: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
//  name:password:UID:GID:GECOS:directory:shell
string LinuxParser::User(int pid) { 
    string target_uid = Uid(pid);
    string line;
    string name, pw, uid;
    
    std::ifstream stream(kPasswordPath);
    if (stream.is_open()) {
      while(std::getline(stream, line)) {
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        while (linestream >> name >> pw >> uid) {
            if (uid == target_uid) {
                stream.close();
                return name;
            }
        }
      }
    }
    return name;  
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) { 
    vector<string> process_stat = GetProcStatus(pid);
    // std::cout<<process_stat.size()<<std::endl;
    return stol(process_stat[21]) / sysconf(_SC_CLK_TCK);
}

std::vector<std::string> LinuxParser::GetProcStatus(int pid) {
    vector<string> process_stat;
    std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
    string line;
    string value;
    if (stream.is_open()) {
      while(std::getline(stream, line)) {
        std::istringstream linestream(line);
        while (linestream >> value) {
            process_stat.push_back(value);
        }
      }
    }
    stream.close();
    // std::cout<<process_stat.size()<<std::endl;
    return process_stat;
}
