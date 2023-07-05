#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "../include/process.h"
#include "../include/processor.h"
#include "../include/system.h"
#include "../include/linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// Return a container composed of the system's processes
vector<Process>& System::Processes() {
  processes_ = {};
  auto pids = LinuxParser::Pids();
  for (auto pid : pids) {
    Process new_process(pid);
    processes_.push_back(new_process);
    if ( processes_.size() >= 100 )
      break;
  }
  return processes_;
}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// Return the number of seconds since the system started running
long System::UpTime() { return LinuxParser::UpTime(); }
