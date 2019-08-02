#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "ProcessParser.h"
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
     Process(string pid)
        {
            this->pid = pid;
            this->user = ProcessParser::getProcUser(pid);
            this->mem = ProcessParser::getVmsSize(pid);
            this->cmd = ProcessParser::getCmd(pid);
            this->up_time = ProcessParser::getProcUpTime(pid);
            this->cpu  = ProcessParser::getCpuPercent(pid);
        }
        void setPid(int pid);
        string getPid() const;
        string getUser() const;
        string getCmd() const;
        int getCpu() const;
        int getMem() const;
        string getUpTime() const;
        string getProcess();
};

#endif
