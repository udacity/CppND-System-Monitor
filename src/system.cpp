#include "system.h"

#include <unistd.h>

#include <cstddef>
#include <fstream>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
/*You need to complete the mentioned TODOs in order to satisfy the rubric
criteria "The student will be able to extract and display basic data about the
system."

You need to properly format the uptime. Refer to the comments mentioned in
format. cpp for formatting the uptime.*/

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() {
  vector<int> processesPids = LinuxParser::Pids();
  System::updateSystemReadings();

  for (auto& pid : processesPids) {
    auto it = std::find(processes_.begin(), processes_.end(), Process(pid));
    if (std::end(processes_) != it) {
      it->KeepAlive(true);
    } else {
      processes_.emplace_back(Process(pid));
    }
  }
  // LinuxParser::CpuUtilization();
  for (auto process = processes_.begin(); process != std::end(processes_);) {
    if (!(process->KeepAlive())) {
      processes_.erase(process);
    } else {
      process->UpdateProcess();
      process->KeepAlive(false);
      ++process;
    }
  }
  std::sort(processes_.begin(), processes_.end(),
            [](auto& a, auto& b) { return a < b; });
  return processes_;
}

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {
  if (!kernelRead_) {
    kernelRead_ = true;
    systemKernel_ = LinuxParser::Kernel();
  }
  return systemKernel_;
}

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return memoryUtilization_; }

// DONE: Return the operating system name
std::string System::OperatingSystem() {
  if (!osRead_) {
    osRead_ = true;
    systemOS_ = LinuxParser::OperatingSystem();
  }
  return systemOS_;
}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return runningProcesses_; }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return totalProcesses_; }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return upTime_; }

void System::updateSystemReadings() {
  updateMemoryUtilization();
  updateUpTime();
  updateTotalProcesses();
  updateRunningProcesses();
}
void System::updateMemoryUtilization() {
  memoryUtilization_ = LinuxParser::MemoryUtilization();
}
void System::updateUpTime() { upTime_ = LinuxParser::UpTime(); }
void System::updateTotalProcesses() {
  totalProcesses_ = LinuxParser::TotalProcesses();
}
void System::updateRunningProcesses() {
  runningProcesses_ = LinuxParser::RunningProcesses();
}