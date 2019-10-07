#include "processor.h"
#include <string>
#include <vector>
#include "constants.h"

using namespace std;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { return 0.0; }

float get_sys_active_cpu_time1(vector<string> values) {
  return (stof(values[S_USER]) + stof(values[S_NICE]) + stof(values[S_SYSTEM]) +
          stof(values[S_IRQ]) + stof(values[S_SOFTIRQ]) +
          stof(values[S_STEAL]) + stof(values[S_GUEST]) +
          stof(values[S_GUEST_NICE]));
}

float get_sys_idle_cpu_time1(vector<string> values) {
  return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}

std::string Processor::printCpuStats(std::vector<std::string> values1,
                                    std::vector<std::string> values2) {
  /*
  Because CPU stats can be calculated only if you take measures in two different
  time, this function has two parameters: two vectors of relevant values. We use
  a formula to calculate overall activity of processor.
  */
  float result = 0;
  float activeTime =
      get_sys_active_cpu_time1(values2) - get_sys_active_cpu_time1(values1);
  float idleTime =
      get_sys_idle_cpu_time1(values2) - get_sys_idle_cpu_time1(values1);
  float totalTime = activeTime + idleTime;
  if(totalTime != 0)
  {
    result = 100.0 * (activeTime / totalTime);
  }
  return std::to_string(result);
}