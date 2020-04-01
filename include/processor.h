#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <functional>
#include <string>
#include <vector>

class Processor
{
 public:
  float Utilization();  // TODO: See src/processor.cpp

  Processor() = default;

  Processor(const std::vector<std::string>& v)
  {
    if (v.size() != 10) return;
    std::vector<std::reference_wrapper<long>> values{
        user, nice,    system, idle,  iowait,
        irq,  softirq, steal,  guest, guest_nice};
    for (size_t i = 0; i < v.size(); i++)
    {
      values.at(i).get() = std::stoi(v.at(i));
    }
  }

  // private:
  long user{};
  long nice{};
  long system{};
  long idle{};
  long iowait{};
  long irq{};
  long softirq{};
  long steal{};
  long guest{};
  long guest_nice{};
};

#endif