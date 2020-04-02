#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class System;

class Process
{
 public:
  Process(System& system, int pid);

  int Pid();
  std::string User() const;
  std::string Command() const;
  float CpuUtilization() const;
  std::string Ram() const;
  long int UpTime() const;
  bool operator<(const Process& other) const;

 private:
  System& system_;
  int pid_;
};

#endif