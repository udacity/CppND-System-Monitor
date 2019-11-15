#ifndef PROCESS_H
#define PROCESS_H

#include <string>
using std::string;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  int Pid() const;
  string User();
  string Command();
  float CpuUtilization() const;
  string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  Process(int pid) : pid_(pid) {};

 private:
   int pid_;
};

#endif
