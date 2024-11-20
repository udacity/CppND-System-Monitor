#include <string>
#include <vector>
#include "linux_parser.h"

using std::string;
using std::stof;
using std::vector;
//==

#include "processor.h"

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 

  vector<string> cpu;
  float total, idle, cpu_percent;

  cpu = LinuxParser::CpuUtilization();
  total = stof(cpu[0]) + stof(cpu[1]) + stof(cpu[2]) + stof(cpu[5]) + stof(cpu[6]) + stof(cpu[7]);
  idle = stof(cpu[3]) + stof(cpu[4]);
  total += idle;
  cpu_percent = (total - idle) / total;     // calc utilization based on stack overflow discussion

  return cpu_percent;
}