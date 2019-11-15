#ifndef PROCESSOR_H
#include "linux_parser.h"
#include "process.h"
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();
  std::vector<Process> Processes();

 private:
};

#endif
