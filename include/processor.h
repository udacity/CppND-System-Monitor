#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

using namespace std;

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  static std::string printCpuStats(std::vector<std::string> values1, std::vector<std::string> values2);

  // TODO: Declare any necessary private members
 private:
};

#endif