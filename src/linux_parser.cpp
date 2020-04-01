#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem()
{
  string line;
  std::regex rgx{"PRETTY_NAME=\"(.*)\""};
  std::ifstream filestream(kOSPath);
  if (filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::smatch matches;
      if (std::regex_search(line, matches, rgx))
      {
        return matches[1];
      }
    }
  }
  return {};
}

string LinuxParser::Kernel()
{
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  for (std::filesystem::directory_entry file :
       std::filesystem::directory_iterator(kProcDirectory))
  {
    if (file.is_directory())
    {
      auto filename = file.path().filename().string();
      if (find_if(filename.begin(), filename.end(), [](const auto& ch) {
            return !std::isdigit(ch);
          }) == filename.end())
      {
        pids.push_back(std::stoi(filename));
      }
    }
  }
  return pids;
}

float LinuxParser::MemoryUtilization()
{
  std::ifstream file{kProcDirectory + kMeminfoFilename};
  if (file)
  {
    auto extract_memory_line = [&file]() {
      float value{};
      std::string line{};
      std::getline(file, line);
      std::stringstream ss{line};
      ss.ignore(std::numeric_limits<std::streamsize>::max(), ' ');
      ss >> value;
      return value;
    };

    float total_memory = extract_memory_line();
    float free_memory = extract_memory_line();

    file.close();
    return total_memory - free_memory;
  }

  return {0.f};
}

long LinuxParser::UpTime() { 
  std::ifstream file{kProcDirectory + kUptimeFilename};
  if (file)
  {
      long uptime{};
      std::string line{};
      std::getline(file, line);
      std::stringstream ss{line};
      ss >> uptime;
      return uptime;
      file.close();
  }
  return 0;
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

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { return 0; }

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { return 0; }

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