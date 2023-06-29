#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

Process::Process(int pid) : pid_(pid){}

int Process::Pid()
{
  return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

std::string Process::Command()
{
  return LinuxParser::Command(pid_);
}

// TODO: Return this process's memory utilization
std::string Process::Ram() { return std::string(); }

// TODO: Return the user (name) that generated this process
std::string Process::User() { return std::string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }