#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

  // DONE: Define any necessary private members
 private:
  Processor cpu_;
  std::vector<Process> processes_ = {};
  std::string systemOS_;
  bool osRead_{false};
  std::string systemKernel_;
  bool kernelRead_{false};
  float memoryUtilization_;
  long upTime_;
  int totalProcesses_;
  int runningProcesses_;

  void updateMemoryUtilization();
  void updateUpTime();
  void updateTotalProcesses();
  void updateRunningProcesses();
  void updateSystemReadings();
};

#endif