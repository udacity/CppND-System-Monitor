#include <string>
#include <vector>
#include "../include/processor.h"
#include "../include/linux_parser.h"

// Read from /proc/stat and store in private member data
void Processor::ReadStatus() {
  std::vector<std::string> cpu_values_str = LinuxParser::CpuUtilization();

  // Before processing new data, move current to previous
  previous_ = current_;

  // Convert values and assign to member data
  current_.user = std::stoi(cpu_values_str[0]);
  current_.nice = std::stoi(cpu_values_str[1]);
  current_.system = std::stoi(cpu_values_str[2]);
  current_.idle = std::stoi(cpu_values_str[3]);
  current_.iowait = std::stoi(cpu_values_str[4]);
  current_.irq = std::stoi(cpu_values_str[5]);
  current_.softirq = std::stoi(cpu_values_str[6]);
  current_.steal = std::stoi(cpu_values_str[7]);
  current_.guest = std::stoi(cpu_values_str[8]);
  current_.guestnice = std::stoi(cpu_values_str[9]);
}

// Return the aggregate CPU utilization
float Processor::Utilization() {
  unsigned long long int prevIdle;
  unsigned long long int idle;
  unsigned long long int prevNonIdle;
  unsigned long long int nonIdle;
  unsigned long long int prevTotal;
  unsigned long long int total;
  unsigned long long int totald;
  unsigned long long int idled;

  // Get latest CPU stats
  ReadStatus();

  // Calculate CPU usage per
  // https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
  prevIdle = previous_.idle + previous_.iowait;
  idle = current_.idle + current_.iowait;
  prevNonIdle =
    previous_.user +
    previous_.nice +
    previous_.system +
    previous_.irq +
    previous_.softirq +
    previous_.steal;
  nonIdle = current_.user +
    current_.nice +
    current_.system +
    current_.irq +
    current_.softirq +
    current_.steal;
  prevTotal = prevIdle + prevNonIdle;
  total = idle + nonIdle;
  totald = total - prevTotal;
  idled = idle - prevIdle;

  if ( totald == 0 )
    return 0.0;
  return (totald - idled) / static_cast<float>(totald);
}
