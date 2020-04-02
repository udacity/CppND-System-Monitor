#include "system.h"

#include <unistd.h>

#include <cstddef>
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

System::System()
{
    auto pids = LinuxParser::Pids();
    for(const auto& pid : pids)
    {
        processes_.emplace_back(*this, pid);
    }
}

const Processor& System::Cpu() { return cpu_; }

const vector<Process>& System::Processes() { return processes_; }

std::string System::Kernel() const { return LinuxParser::Kernel(); }

float System::MemoryUtilization() const
{
  auto mem_info = LinuxParser::MemoryUtilization();
  auto total_memory = mem_info.first;
  auto free_memory = mem_info.second;
  return (total_memory - free_memory) / total_memory;
}

std::string System::OperatingSystem() const { return LinuxParser::OperatingSystem(); }

int System::RunningProcesses() const { return LinuxParser::RunningProcesses(); }

int System::TotalProcesses() const { return LinuxParser::TotalProcesses(); }

long int System::UpTime() const { return LinuxParser::UpTime(); }