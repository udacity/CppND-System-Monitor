#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
public:
  /**
   * @brief Getter for cpu_ member
   *
   * @return Processor object cpu_
   */
  Processor& Cpu();

  /**
   * @brief Builds a vector of Process objects using the process ids
   *
   * @return Vector of Process objects (one for each process)
   */
  std::vector<Process>& Processes();

  /**
   * @brief Returns memory utilization of the system in % (0-1) via linux_parser MemoryUtilization()
   *
   * @return float with memory utilization of system in percentage (0-1)
   */
  float MemoryUtilization();

  /**
   * @brief This returns the uptime of the system in secs (a passthrough to linux_parser UpTime())
   *
   * @return long with uptime of system in seconds
   */
  long UpTime();

  /**
   * @brief Returns the total number of processes (a passthrough to linux_parser TotalProcesses())
   *
   * @return int with total number of processes
   */
  int TotalProcesses();

  /**
   * @brief Returns the number of processes running (passthrough to linux_parser RunningProcesses())
   *
   * @return int with number of processes running
   */
  int RunningProcesses();

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

private:
  Processor cpu_;
  std::vector<Process> processes_;
};

#endif
