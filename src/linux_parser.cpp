#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

#define USER (0)
#define NICE (1)
#define SYSTEM (2)
#define IDLE (3)
#define IOWAIT (4)
#define IRQ (5)
#define SOFTIRQ (6)
#define STEAL (7)
#define GUEST (8)
#define GUEST_NICE (9)
#define NO_OF_JIFFIES (10)

static vector<string> Jiffies_s(10, "");

bool searchString_helper(string regex_xpr, const string searchString,
                         unsigned int index, std::string& str) {
  std::smatch regexMatch;
  // look for regex
  if (std::regex_match(searchString, regexMatch, std::regex(regex_xpr))) {
    // The first sub_match is the whole string; the next
    // sub_match is the first parenthesized expression.
    if (regexMatch.size() > index) {
      str = regexMatch[index].str();
      return true;
    }
  }
  return false;
}

void findSysJiffies() {
  std::ifstream statFile(
      LinuxParser::kProcDirectory + LinuxParser::kStatFilename, std::ios::in);

  if (!statFile) {
    return;
  }
  while (statFile) {
    std::string searchString;
    std::getline(statFile, searchString);
    std::smatch regexMatch;
    // look for regex
    if (std::regex_match(
            searchString, regexMatch,
            std::regex(
                "cpu\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+"
                ")\\s+(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+).*"))) {
      // The first sub_match is the whole string; the next
      // sub_match is the first parenthesized expression.
      if (regexMatch.size() == NO_OF_JIFFIES + 1) {
        int i{0};
        for (auto& match : regexMatch) {
          if (&match == &regexMatch[0]) {
            continue;
          }
          Jiffies_s[i++] = match;
        }
        break;
      }
    }
  }
}
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

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::string totalMem;
  std::string freeMem;
  float memUtilization;
  vector<bool> isFound(2, false);

  std::ifstream memInfo(kProcDirectory + kMeminfoFilename, std::ios::in);

  if (!memInfo) {
    return 0.0;
  }
  // read Meminfo file
  while (memInfo && (!isFound[0] || !isFound[1])) {
    std::string lineBuffer;
    std::getline(memInfo, lineBuffer);
    // look for total mem
    isFound[0] = isFound[0]
                     ? isFound[0]
                     : searchString_helper("^MemTotal:\\s*([\\d]+)\\s*.*",
                                           lineBuffer, 1, totalMem);
    // look for free mem
    isFound[1] = isFound[1] ? isFound[1]
                            : searchString_helper("^MemFree:\\s*([\\d]+)\\s*.*",
                                                  lineBuffer, 1, freeMem);
  }
  if (!isFound[0] || !isFound[1]) {
    memUtilization = 0.0;
  } else {
    memUtilization = std::stoll(totalMem) - std::stoll(freeMem);
    memUtilization /= std::stoll(totalMem);
  }
  return memUtilization;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  std::ifstream upTimeInfo(kProcDirectory + kUptimeFilename, std::ios::in);
  string upTime;

  if (!upTimeInfo) {
    return 0.0;
  }
  // read Meminfo file
  while (upTimeInfo) {
    std::string lineBuffer;
    std::getline(upTimeInfo, lineBuffer);
    if (searchString_helper("^([\\d]+)\\..*", lineBuffer, 1, upTime)) {
      return (std::stol(upTime));
    }
  }
  return 0;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return (ActiveJiffies() + IdleJiffies()); }

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  vector<string> pidJiffies = CpuUtilization(pid);
  if (pidJiffies.size() != 0)
    return (std::stol(pidJiffies[0]) + std::stol(pidJiffies[1]) +
            std::stol(pidJiffies[2]) + std::stol(pidJiffies[3]));
  else {
    return 0.0;
  }
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return (std::stof(Jiffies_s[USER]) + std::stof(Jiffies_s[NICE]) +
          std::stof(Jiffies_s[SYSTEM]) + std::stof(Jiffies_s[IRQ]) +
          std::stof(Jiffies_s[SOFTIRQ]) + std::stof(Jiffies_s[GUEST]) +
          std::stof(Jiffies_s[GUEST_NICE]));
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  return (std::stof(Jiffies_s[IDLE]) + std::stof(Jiffies_s[IOWAIT]) +
          std::stof(Jiffies_s[STEAL]));
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  findSysJiffies();
  return Jiffies_s;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream totalProcessFile(kProcDirectory + kStatFilename, std::ios::in);
  string totalProcess;

  if (!totalProcessFile) {
    return 0.0;
  }
  // read Meminfo file
  while (totalProcessFile) {
    std::string lineBuffer;
    std::getline(totalProcessFile, lineBuffer);
    if (searchString_helper("^processes\\s*([\\d]+).*", lineBuffer, 1,
                            totalProcess)) {
      return (std::stoi(totalProcess));
    }
  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  std::ifstream runningProcessFile(kProcDirectory + kStatFilename,
                                   std::ios::in);
  string runningProcess;

  if (!runningProcessFile) {
    return 0.0;
  }
  // read Meminfo file
  while (runningProcessFile) {
    std::string lineBuffer;
    std::getline(runningProcessFile, lineBuffer);
    if (searchString_helper("^procs_running\\s*([\\d]+).*", lineBuffer, 1,
                            runningProcess)) {
      return (std::stoi(runningProcess));
    }
  }
  return 0;
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::ifstream commandFile(kProcDirectory + to_string(pid) + kCmdlineFilename,
                            std::ios::in);
  string cmd = " ";

  if (!commandFile) {
    return "0";
  }
  std::getline(commandFile, cmd);
  return cmd;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid [[maybe_unused]]) {
  std::ifstream runningProcessFile(
      kProcDirectory + to_string(pid) + kStatusFilename, std::ios::in);
  string vMem;

  if (!runningProcessFile) {
    return "0";
  }
  // read Meminfo file
  while (runningProcessFile) {
    std::string lineBuffer;
    std::getline(runningProcessFile, lineBuffer);
    if (searchString_helper("^VmSize:\\s+(\\d+)\\s+kB.*", lineBuffer, 1,
                            vMem)) {
      return vMem;
    }
  }
  return string(0);
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream runningProcessFile(
      kProcDirectory + to_string(pid) + kStatusFilename, std::ios::in);
  string pUid;

  if (!runningProcessFile) {
    return "";
  }
  // read Meminfo file
  while (runningProcessFile) {
    std::string lineBuffer;
    std::getline(runningProcessFile, lineBuffer);
    if (searchString_helper("^Uid:\\s+(\\d+).*", lineBuffer, 1, pUid)) {
      return pUid;
    }
  }
  return string();
}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::ifstream passwdFile(kPasswordPath, std::ios::in);
  string pUid = LinuxParser::Uid(pid);
  string userName;

  if (!passwdFile) {
    return "";
  }
  // read Meminfo file
  while (passwdFile) {
    std::string lineBuffer;
    std::getline(passwdFile, lineBuffer);
    if (searchString_helper("^([\\w-]+):.*:(?:" + pUid + "):.*", lineBuffer, 1,
                            userName)) {
      return userName;
    }
  }
  return string();
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  std::ifstream statFile(LinuxParser::kProcDirectory + std::to_string(pid) +
                             LinuxParser::kStatFilename,
                         std::ios::in);
  long startTime;
  if (!statFile) {
    return {};
  }
  while (statFile) {
    std::string searchString;
    std::getline(statFile, searchString);
    std::smatch regexMatch;
    // look for regex
    if (std::regex_match(searchString, regexMatch,
                         std::regex("^(?:[\\S]+\\s+){21}(\\d+)\\s+.*"))) {
      // The first sub_match is the whole string; the next
      // sub_match is the first parenthesized expression.
      if (regexMatch.size() > 1) {
        startTime = std::stol(regexMatch[1]);
        break;
      }
    }
  }
  return startTime;
}

std::vector<std::string> LinuxParser::CpuUtilization(int pid) {
  std::ifstream statFile(LinuxParser::kProcDirectory + std::to_string(pid) +
                             LinuxParser::kStatFilename,
                         std::ios::in);
  vector<string> utilization;
  if (!statFile) {
    return {};
  }
  while (statFile) {
    std::string searchString;
    std::getline(statFile, searchString);
    std::smatch regexMatch;
    // look for regex
    if (std::regex_match(
            searchString, regexMatch,
            std::regex(
                "^(?:[\\S]+\\s+){13}(\\d+)\\s+(\\d+)\\s+(\\d+)\\s+(\\d+).*"))) {
      // The first sub_match is the whole string; the next
      // sub_match is the first parenthesized expression.
      if (regexMatch.size() > 1) {
        for (auto& match : regexMatch) {
          if (&match == &regexMatch[0]) {
            continue;
          }
          utilization.push_back(match);
        }
        break;
      }
    }
  }
  return utilization;
}