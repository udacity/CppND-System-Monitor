#ifndef PROCESS_H_
#define PROCESS_H_

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid) {}
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();
  long int UpTime();                       // TODO: See src/process.cpp
  void Pid(int p) { pid_ = p; }
  // bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 private:
  int pid_{0};

};

#endif  // PROCESS_H_

// TODO: 
// Idea 1: create class data, upon construction, populate everything.  Getters return class data.
// - this allows sorting within system class by whatever we want
// - seems cleaner
// - time consuming for thousands of files?
// Idea 2: only hold pid as class data.  Get info from files only when calls are made by display code.
// - faster
// - not sure how to sort...system would have to get data and track it itself?  seems awkward