#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization()
{
  float tot_jiffies = static_cast<float>(LinuxParser::Jiffies());
  float act_jiffies = static_cast<float>(LinuxParser::ActiveJiffies());

  float total_delta = tot_jiffies - prev_tot_jiffies_;
  float act_delta =  act_jiffies - prev_act_jiffies_;

  prev_tot_jiffies_ = tot_jiffies;
  prev_act_jiffies_ = act_jiffies;
  
  return act_delta / total_delta;
}
