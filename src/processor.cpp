#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  std::vector<std::string> cpuUtilization; 
  cpuUtilization = LinuxParser::CpuUtilization(); //return the vector of strings with the 10 values beside cpu
  //getting the current values
  float curr_user = stof(cpuUtilization[0]);
  float curr_nice = stof(cpuUtilization[1]);
  float curr_system = stof(cpuUtilization[2]);
  float curr_idle = stof(cpuUtilization[3]);
  float curr_iowait = stof(cpuUtilization[4]);
  float curr_irq = stof(cpuUtilization[5]);
  float curr_softirq = stof(cpuUtilization[6]);
  float curr_steal = stof(cpuUtilization[7]);
  //following the stackoverflow equations for calculating	
  float prevIdle = prev_idle_ + prev_iowait_;
  float idle = curr_idle + curr_iowait;

  float prevNonIdle = prev_user_ + prev_nice_ + prev_system_ + prev_irq_ + prev_softirq_ + prev_steal_;
  float nonIdle = curr_user + curr_nice + curr_system + curr_irq + curr_softirq + curr_steal;

  float prevTotal = prevIdle + prevNonIdle;
  float total = idle + nonIdle;

  double totald = total - prevTotal;

  float idled = idle - prevIdle;

  float cpuPercentage = (totald - idled) / totald;

  // we have to update the previous values to be current for the next calculation
  PrevUser(curr_user);
  PrevNice(curr_nice);
  PrevSystem(curr_system);
  PrevIdle(curr_idle);
  PrevIowait(curr_iowait);
  PrevIRQ(curr_irq);
  PrevSoftIRQ(curr_softirq);
  PrevSteal(curr_steal);

  return cpuPercentage;
}
  