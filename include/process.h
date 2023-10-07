#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  bool operator<(Process const& a) const;
  bool operator==(Process const& a) const;
  bool KeepAlive();
  void KeepAlive(bool);
  void UpdateProcess();

  // DONE: Declare any necessary private members
 private:
    int pid_;
    bool keepAlive_;
    std::string memUsage_;
    float cpuUsage_;
    float currentActiveJiffies_;
    float currentTotalJiffies_;
    std::string username_;
    std::string command_;
    bool userInitialized_;
    bool commandInitialized_;
    long upTime_;

    void updateUpTime();
    void updateCpuUsage(void);
    void updateMemUsage(void);
    
};

#endif