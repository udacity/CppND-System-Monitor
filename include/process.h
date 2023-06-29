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
  std::string User();                      // TODO: See src/process.cpp

  /**
   * @brief This returns the command string associated with pid_ (passthrough to LinuxParser)
   * 
   * @return command string associated with pid_
   */
  std::string Command();

  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int pid_;
};

#endif