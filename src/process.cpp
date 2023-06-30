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

float Process::CpuUtilization()
{
  long total_time = LinuxParser::ActiveJiffies(pid_);
  long seconds = UpTime();
  return (static_cast<float>(total_time) / sysconf(_SC_CLK_TCK)) / static_cast<float>(seconds);
}

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

long Process::UpTime()
{
  return LinuxParser::UpTime(pid_);
}

bool Process::operator<(Process const& a) const
{
  // Return true if the passed in process uses less ram
  return std::stoi(a.Ram()) < std::stoi(this->Ram());
}
