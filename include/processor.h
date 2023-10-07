#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>
#include <string>


class Processor {
 public:
  float Utilization();
  Processor();

  // DONE: Declare any necessary private members
 private:
 static bool IsInitializied_;
 float CurrentRunTime_;
 float CurrentTotalTime_;
};

#endif