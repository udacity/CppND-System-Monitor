#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;
/*
// TODO: Return this process's ID
int Process::Pid() { return 0; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() { return string(); }

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { return string(); }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return 0; }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
*/
void Process::setPid(int pid)
{
    this->pid = pid;
}
string Process::getPid() const 
{
    return this->pid;
}

string Process::getUser() const
{
    return this->user;
}

int Process::getMem() const
{
    return std::stoi(this->mem);
}

string Process::getUpTime() const
{
    return this->up_time;
}

int Process::getCpu() const
{
    return std::stoi(this->mem);
}

string Process::getProcess()
{
    this->mem = ProcessParser::getVmsSize(this->pid);
    this->up_time = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);

    return (this->pid + "   "
                    + this->user
                    + "   "
                    + this->mem.substr(0,5)
                    + "     "
                    + this->cpu.substr(0,5)
                    + "     "
                    + this->up_time.substr(0,5)
                    + "    "
                    + this->cmd.substr(0,30)
                    + "...");
}
