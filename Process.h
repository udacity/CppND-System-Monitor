#pragma once

#include <string>
#include "ProcessParser.h"
#include "util.h"

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
    float cpu;
    string mem;
    float upTime;

public:
    Process(string pid){
        this->pid = pid;
        this->user = ProcessParser::getProcUser(pid);
        this->mem = ProcessParser::getVmSize(pid);
        this->cmd = ProcessParser::getCmd(pid);
        this->upTime = ProcessParser::getProcUpTime(pid);
        this->cpu = ProcessParser::getCpuPercent(pid);

    }
    void setPid(int pid);
    string getPid()const;
    string getUser()const;
    string getCmd()const;
    float getCpu()const;
    string getMem()const;
    float getUpTime()const;
    string getProcess();
};
void Process::setPid(int pid){
    this->pid = pid;
}
string Process::getPid()const {
    return this->pid;
}

string Process::getCmd()const {
    return this->cmd;
}

float Process::getCpu()const {
    return this->cpu;
}

string Process::getUser()const {
    return this->user;
}

float Process::getUpTime()const {
    return this->upTime;
}

string Process::getMem()const {
    return this->mem;
}



string Process::getProcess(){
    if(!ProcessParser::isPidExisting(this->pid))
        return "";
    mem = ProcessParser::getVmSize(pid);
    upTime = ProcessParser::getProcUpTime(pid);
    cpu = ProcessParser::getCpuPercent(pid);
    stringstream out;
    out << setprecision(2) << fixed;
    out << setw(6) << right << pid << " " 
        << setw(14) << left << user.substr(0,14) << " " 
        << setw(7) << right  << cpu << " "
        << setw(15) << right  << mem << " "
        << setw(9) << right << Util::convertToTime((long)(upTime)) << "  "
        << setw(30) << left << cmd.substr(0,50);
    return out.str();
    //return (pid + "   " + user + "   " + mem + "   " + cpu + "   " + upTime + "   " + cmd.substr(0,30));
}
