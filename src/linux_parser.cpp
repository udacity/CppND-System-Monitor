#include "linux_parser.h"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

std::string LinuxParser::OperatingSystem()
{
  return MatchStringInFile(kOSPath, std::regex{"PRETTY_NAME=\"(.*)\""});
}

std::string LinuxParser::Kernel()
{
  std::string os{}, kernel{};
  std::string line{};
  std::ifstream file(kProcDirectory + kVersionFilename);
  if (file.is_open())
  {
    std::getline(file, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
    file.close();
  }
  return kernel;
}

std::vector<int> LinuxParser::Pids()
{
  std::vector<int> pids;
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
  float memory_usage{};
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
    memory_usage = total_memory - free_memory;

    file.close();
  }

  return memory_usage;
}

long LinuxParser::UpTime()
{
  long uptime{};
  std::ifstream file{kProcDirectory + kUptimeFilename};
  if (file.is_open())
  {
    std::string line{};
    std::getline(file, line);
    std::stringstream ss{line};
    ss >> uptime;
    file.close();
  }
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

std::vector<std::string> LinuxParser::CpuUtilization()
{
  std::vector<std::string> cpus_values{};
  std::regex rgx{"cpu[[:digit:]]*(( )? [[:digit:]]+){10}"};
  std::ifstream file(kProcDirectory + kStatFilename);
  if (file.is_open())
  {
    std::string line{};
    while (std::getline(file, line))
    {
      std::smatch matches;
      if (!std::regex_search(line, matches, rgx))
      {
        break;
      }
      cpus_values.push_back(matches[0]);
    }
  }
  return cpus_values;
}

int LinuxParser::TotalProcesses()
{
  return std::stoi(MatchStringInFile(kProcDirectory + kStatFilename,
                                     std::regex{"processes ([[:digit:]]+)"}));
}

int LinuxParser::RunningProcesses()
{
  return std::stoi(
      MatchStringInFile(kProcDirectory + kStatFilename,
                        std::regex{"procs_running ([[:digit:]]+)"}));
}

std::string LinuxParser::Command(int pid)
{
  std::string cmd{};
  std::ifstream file(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (file.is_open())
  {
    std::getline(file, cmd);
    file.close();
  }
  return cmd;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
std::string LinuxParser::Ram(int pid[[maybe_unused]]) { return std::string(); }

std::string LinuxParser::Uid(int pid)
{
  return MatchStringInFile(
      kProcDirectory + std::to_string(pid) + kStatusFilename,
      std::regex{"Uid:\\s+([[:digit:]]+)"});
}

std::string LinuxParser::User(int pid)
{
  return MatchStringInFile(
      kPasswordPath,
      std::regex{"([[:alnum:]]+):x:" + LinuxParser::Uid(pid) + ":"});
}

long LinuxParser::UpTime(int pid)
{
  return std::stoi(MatchStringInFile(
      kProcDirectory + std::to_string(pid) + kStatFilename,
      std::regex{"[[:digit:]]+ \\([[:alnum:]]*\\) [[:alpha:]] (?:[[:digit:]]+ "
                 "){18}([[:digit:]]+)"}));
}

std::string LinuxParser::MatchStringInFile(std::string filename, std::regex rgx)
{
  std::string line{}, match{};
  std::ifstream file(filename);
  if (file.is_open())
  {
    while (std::getline(file, line))
    {
      std::smatch matches;
      if (std::regex_search(line, matches, rgx))
      {
        file.close();
        match = matches[1];
        break;
      }
    }
  }
  return match;
}