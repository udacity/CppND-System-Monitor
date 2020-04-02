#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class System;

class Process
{
 public:
  Process(System& system, int pid);

  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process& other);

 private:
  System& system_;
  int pid_;
};

#endif