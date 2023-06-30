#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

Process::Process(int pid) : pid_(pid) {}

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

std::string Process::Ram() const
{
  return LinuxParser::Ram(pid_);
}

std::string Process::User()
{
  return LinuxParser::User(pid_);
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

bool Process::operator<(Process const& a) const
{
  // Return true if the passed in process uses less ram
  return std::stoi(a.Ram()) < std::stoi(this->Ram());
}
