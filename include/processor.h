#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>

class Processor
{
 public:
  Processor() = default;

  Processor(const std::string& values);

  float Utilization();  // TODO: See src/processor.cpp

 private:
  std::string cpu_name_{};
  long user_{};
  long nice_{};
  long system_{};
  long idle_{};
  long iowait_{};
  long irq_{};
  long softirq_{};
  long steal_{};
  long guest_{};
  long guest_nice_{};
};

#endif