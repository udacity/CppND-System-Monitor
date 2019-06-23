#include <string>
#include "ProcessParser.h"

using namespace std;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
private:
    string pid;
    string user;
    string cmd;
    string cpu;
    string mem;
    string upTime;

public:
    Process(string pid){
        this->pid = pid;
        this->user = ProcessParser::getProcUser(pid);
        this->mem = ProcessParser::getVmSize(pid);
        this->cmd = ProcessParser::getCmd(pid);
        this->cpu = ProcessParser::getCpuPercent(pid);
        this->upTime = ProcessParser::getProcUpTime(pid);
    }
    void setPid(int pid){ this->pid = pid; }
    string getPid()const { return this->pid; }
    string getUser()const { return this->user; }
    string getCmd()const { return this->cmd; }
    string getCpu()const {return this->cpu; }
    string getMem()const {return this->mem; }
    string getUpTime()const {return this->upTime; }
    string getProcess();
};

string Process::getProcess(){
    //if process exists, update current info
    if(!ProcessParser::isPidExisting(this->pid))
        return "";
    this->mem = ProcessParser::getVmSize(this->pid);
    this->upTime = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);

    return this->pid + "   " + this->user + "   " +
           this->cpu.substr(0,5) + "   " +this->mem.substr(0,5) + "   " +  
           this->upTime.substr(0,5) + "   " +  this->cmd.substr(0,30);
}