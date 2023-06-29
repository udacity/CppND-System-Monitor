#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu()
{
  return cpu_;
}

vector<Process>& System::Processes()
{
  std::vector<int> pids = LinuxParser::Pids();
  for (int pid: pids) {
    processes_.push_back(Process(pid));
  }
  return processes_;
}

std::string System::Kernel()
{
  return LinuxParser::Kernel();
}

float System::MemoryUtilization()
{
  return LinuxParser::MemoryUtilization();
}

std::string System::OperatingSystem() 
{
  return LinuxParser::OperatingSystem();
}

int System::RunningProcesses()
{
  return LinuxParser::RunningProcesses();
}

int System::TotalProcesses()
{
  return LinuxParser::TotalProcesses();
}

long System::UpTime() 
{
  return LinuxParser::UpTime();
}
