#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid, long hz);//declaring constructor
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process & a);  	   

  // TODO: Declare any necessary private members
 private:
  int pid_;
  long hz_;  //sysconf(_SC_CLK_TCK) 
  float utime_ = 0.0;
  float stime_ = 0.0;
  float cutime_ = 0.0;
  float cstime_ = 0.0;
  float starttime_ = 0.0;
};

#endif