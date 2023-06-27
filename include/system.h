#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();                   // TODO: See src/system.cpp
  std::vector<Process>& Processes();  // TODO: See src/system.cpp
  float MemoryUtilization();          // TODO: See src/system.cpp
  /**
   * @brief This returns the uptime of the system in secs (a passthrough to linux_parser UpTime())
   *
   * @return long with uptime of system in seconds
   */
  long UpTime();
  int TotalProcesses();               // TODO: See src/system.cpp
  int RunningProcesses();             // TODO: See src/system.cpp
  /**
   * @brief This returns the kernel (a passthrough to linux_parser Kernel())
   *
   * @return string with kernel
   */
  std::string Kernel();
  /**
   * @brief This returns the operating system (a passthrough to linux_parser OperatingSystem())
   *
   * @return string with operating system
   */
  std::string OperatingSystem();

  // TODO: Define any necessary private members
 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
};

#endif