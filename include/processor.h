#ifndef PROCESSOR_H
#define PROCESSOR_H
#include "linux_parser.h"
#include<iostream>

struct CPU_INFO {
    unsigned long long int idle_time;
    unsigned long long int non_idle_time;
    unsigned long long int total_time;

};
class Processor {

 public:
    float Utilization();  // TODO: See src/processor.cpp
  // TODO: Declare any necessary private members
 private:
    CPU_INFO GetLatestCpuInfo();
    std::unique_ptr<CPU_INFO> cur_cpu_info_;
    std::unique_ptr<CPU_INFO> prev_cpu_info_;
};

#endif