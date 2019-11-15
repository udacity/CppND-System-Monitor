#include "processor.h"
#include "process.h"
#include <algorithm>

using std::vector;

float Processor::Utilization() { return LinuxParser::CpuUtilization(); }

std::vector<Process> Processor::Processes() {
  vector<Process> processes = {};
  std::vector<int> pids = LinuxParser::Pids();
  auto compare = [](const Process a, const Process b) {  return a < b; };

  for(int id:pids) {
    processes.push_back(Process(id));
  }

  sort(processes.begin(), processes.end(), compare);

  return processes;
}
