#include "processor.h"
#include <sstream>
#include "linux_parser.h"

float Processor::Utilization() const
{
  std::stringstream ss{LinuxParser::CpusUtilization().front()};
  std::string cpu_name{};
  float user{}, nice{}, system{}, idle{}, iowait{}, irq{}, softirq{}, steal{},
      guest{}, guest_nice{};
  ss >> cpu_name >> user >> nice >> system >> idle >> iowait >> irq >>
      softirq >> steal >> guest >> guest_nice;
  float t_idle = idle + iowait;
  float non_idle = user + nice + system + irq + softirq + steal;
  float total = t_idle + non_idle;
  return (total - t_idle) / total;
}