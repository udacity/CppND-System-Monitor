#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

using namespace std;

class System {
 public:
  System();
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  long UpTime();                      // TODO: See src/system.cpp
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  std::string Kernel();               // TODO: See src/system.cpp
  std::string OperatingSystem();      // TODO: See src/system.cpp
  void setAttributes();
  void setCpuCoresStats();
  string getKernelVersion() const;
  string getCpuPercent() const;
  vector<string> getCoresStats() const;
  string getMemPercent() const;
  string getTotalProc() const;
  string getRunningProc() const;
  void getOtherCores(int _size);
  void setLastCpuMeasures();
  void refreshList();
  string printList();
  vector<string> getList();

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  vector<Process> processes_ = {};
  vector<Process> _list;
  vector<string> lastCpuStats;
  vector<string> currentCpuStats;
  vector<string> coresStats;
  vector<vector<string>> lastCpuCoresStats;
  vector<vector<string>> currentCpuCoresStats;
  string cpuPercent;
  float memPercent;
  string osName;
  string kernelVer;
  long upTime;
  int totalProc;
  int runningProc;
  int threads;
};

#endif