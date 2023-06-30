#ifndef LINUX_PARSER_H
#define LINUX_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
/**
 * @brief This returns the memory utilization of the system in percentage (0-1)
 *
 * @return float with memory utilization of system in percentage (0-1)
 */
float MemoryUtilization();

/**
 * @brief This returns the uptime of the system in seconds
 *
 * @return long with uptime of system in seconds
 */
long UpTime();

/**
 * @brief This returns pids of the system in seconds
 *
 * @return vector of pids
 */
std::vector<int> Pids();

/**
 * @brief Returns the total number of processes
 *
 * @return int with total number of processes
 */
int TotalProcesses();

/**
   * @brief Returns the number of processes running
   *
   * @return int with number of processes running
   */
int RunningProcesses();

/**
 * @brief This returns the operating system
 *
 * @return string with operating system
 */
std::string OperatingSystem();

/**
 * @brief This returns the kernel
 *
 * @return string with kernel
 */
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};

/**
 * @brief This returns the 10 elements from the cpu line of /proc/stat
 *
 * @return vec of long where each int corresponds to one of the 10 elements of the cpu utilization
 */
std::vector<long> CpuUtilization();

/**
 * @brief This returns the total number of Jiffies for the system (Active + Idle)
 *
 * @return total number of Jiffies for the system (Active + Idle)
 */
long Jiffies();

/**
 * @brief This returns the total number of active Jiffies for the system
 *
 * @return total number of active Jiffies for the system
 */
long ActiveJiffies();

/**
 * @brief This returns the total number of idle Jiffies for the system
 *
 * @return total number of idle Jiffies for the system
 */
long IdleJiffies();

// Processes
/**
 * @brief This returns the command string that started the process
 *
 * @param[in] pid: process id
 * 
 * @return the command string that started the process
 */
std::string Command(int pid);

/**
 * @brief This returns the ram used by the process
 *
 * @param[in] pid: process id
 * 
 * @return the ram used by the process
 */
std::string Ram(int pid);

/**
 * @brief This returns the uid associated with a process
 *
 * @param[in] pid: process id
 * 
 * @return uid string associated with the pid
 */
std::string Uid(int pid);

/**
 * @brief This returns the user associated with a process
 *
 * @param[in] pid: process id
 * 
 * @return user string associated with the pid
 */
std::string User(int pid);

/**
 * @brief This returns the total number of active Jiffies for the process
 *
 * @param[in] pid: process id
 * 
 * @return total number of active Jiffies for the process
 */
long ActiveJiffies(int pid);

/**
 * @brief Read and return the uptime of a process in seconds
 *
 * @param[in] pid: process id
 * 
 * @return long with uptime of the process in seconds
 */
long UpTime(int pid);
};  // namespace LinuxParser

#endif
