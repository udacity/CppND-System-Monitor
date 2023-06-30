#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
public:
  /**
   * @brief Process constructor
   *
   * @param[in] pid: process id
   *
   * @return Process object
   */
  Process(int pid);
  
  /**
   * @brief Process id getter
   *
   * @return Process id
   */
  int Pid();

  /**
   * @brief This returns the user that started the process
   *
   * @return user that started the process
   */
  std::string User();

  /**
   * @brief This returns the command string associated with pid_ (passthrough to LinuxParser)
   * 
   * @return command string associated with pid_
   */
  std::string Command();

  /**
   * @brief Return this process's CPU utilization (0->1)
   * 
   * @return process's CPU utilization (0->1)
   */
  float CpuUtilization();

  /**
   * @brief This returns the ram used by the process (passthrough to LinuxParser)
   * 
   * @return the ram used by the process
   */
  std::string Ram() const;

  /**
   * @brief Return the age of this process (in seconds) (passthrough to LinuxParser)
   * 
   * @return age of this process (in seconds)
   */
  long UpTime();

  bool operator<(Process const& a) const; // Return true if the ram of a is less than process ram

private:
  int pid_;
};

#endif
