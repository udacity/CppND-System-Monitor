#ifndef PROCESS_H_
#define PROCESS_H_

#include <string>
#include <vector>
#include "../include/linux_parser.h"

using std::string;
using std::vector;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid) : pid_(pid) {
    // read each file one time
    std::vector<std::string> stats = LinuxParser::ReadStat(pid);

    // set local member data to result
    CpuUtilization(stats);

    // todo update get methods to get member data
  }
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  void Pid(int p) { pid_ = p; }
  // bool operator<(Process const& a) const;  // TODO: See src/process.cpp

 private:
  void CpuUtilization(vector<string> stats);
  void UpTime(vector<string> stats);
  int pid_{0};
  float cpu_{0};
  long int uptime_{0};

};

struct PidStat {
  unsigned long long int utime;
  unsigned long long int stime;
  unsigned long long int cutime;
  unsigned long long int cstime;
  unsigned long long int starttime;
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