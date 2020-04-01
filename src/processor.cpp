#include "processor.h"
#include <sstream>

Processor::Processor(const std::string& values)
{
  std::stringstream ss{values};
  ss >> total_.cpu_name_ >> total_.user_ >> total_.nice_ >> total_.system_ >>
      total_.idle_ >> total_.iowait_ >> total_.irq_ >> total_.softirq_ >>
      total_.steal_ >> total_.guest_ >> total_.guest_nice_;
}

float Processor::Utilization()
{
  float idle = total_.idle_ + total_.iowait_;
  float non_idle = total_.user_ + total_.nice_ + total_.system_ + total_.irq_ +
                   total_.softirq_ + total_.steal_;
  float total = idle + non_idle;
  return (total - idle) / total;
}