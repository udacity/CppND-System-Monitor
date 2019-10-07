#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include <fstream>
#include <vector>

using namespace std;

/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
  // TODO: Declare any necessary private members
 private:
  string pid;
  string user;
  string cmd;
  string cpu;
  string mem;
  string up_time;

 public:
  Process(string pid) {
    this->pid = pid;
    this->user = Process::getProcUser(pid);
    this->mem = Process::getVmsSize(pid);
    this->cmd = Process::getCmd(pid);
    this->up_time = Process::getProcUpTime(pid);
    this->cpu = Process::getCpuPercent(pid);
  }

  void setPid(int pid);
  string getPid() const;
  string getUser() const;
  // string getCmd() const;
  int getCpu() const;
  int getMem() const;
  string getUpTime() const;
  string getProcess();
  static string getProcUser(string pid);
  static int getNumberOfRunningProcesses();
  static int getTotalNumberOfProcesses();
  static std::vector<std::string> getPidList();
  static int getTotalThreads();
  static std::string getProcUpTime(std::string pid);
  static std::string getCpuPercent(std::string pid);
  static long int getSysUpTime();
  static std::string getVmsSize(std::string pid);
  static std::string getCmd(std::string pid);
};

#endif
