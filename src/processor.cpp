#include "processor.h"
#include <sstream>

Processor::Processor(const std::string& values)
{
  std::stringstream ss{values};
  ss >> cpu_name_ >> user_ >> nice_ >> system_ >> idle_ >> iowait_ >> irq_ >>
      softirq_ >> steal_ >> guest_ >> guest_nice_;
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { return 0.0; }