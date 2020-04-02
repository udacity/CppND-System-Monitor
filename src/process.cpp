#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(System& system, int pid) : system_(system), pid_(pid) {}

int Process::Pid() { return pid_; }

float Process::CpuUtilization()
{
  auto values = LinuxParser::CpuUtilization(pid_);
  float cpu_usage{};
  if (values.size() == 4)
  {
    float u_time{std::stof(values[0])}, s_time{std::stof(values[1])},
        cu_time{std::stof(values[2])}, cs_time{std::stof(values[3])};

    auto total_time = u_time + s_time + cu_time + cs_time;
    cpu_usage = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / UpTime());
  }
  return cpu_usage;
}

string Process::Command() { return LinuxParser::Command(pid_); }

string Process::Ram() { return LinuxParser::Ram(pid_); }

string Process::User() { return LinuxParser::User(pid_); }

long int Process::UpTime()
{
  return system_.UpTime() - LinuxParser::UpTime(pid_);
}

bool Process::operator<(Process& other) { return Ram() < other.Ram(); }