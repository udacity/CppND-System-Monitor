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
  std::vector<int> pids{LinuxParser::Pids()};

  // Create a set to store and sort the pids
  std::set<int> pids_set;
  // Loop through the processes already stored
  for (Process process : processes_) {
    pids_set.insert(process.Pid());
  }

  for (int pid : pids) {
    // Only add process to the processes_ vector if the pid isn't in the set
    if (pids_set.find(pid) == pids_set.end()){
      processes_.push_back(Process(pid));
    }
  }

  // Sort the processes by RAM
  std::sort(processes_.begin(), processes_.end(), [](Process a, Process b){return a < b;});
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
