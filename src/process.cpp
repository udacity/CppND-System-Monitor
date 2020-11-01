#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) :processID_(pid)
{
    getUser();
    cmd_ = LinuxParser::Command(pid);
    memUtilization();
    GetUpTime();
}

// DONE: Return this process's ID
int Process::Pid() { return processID_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
std::string Process::Command() { return cmd_; }

// TODO: Return this process's memory utilization
std::string Process::Ram() { return mem_; }

// TODO: Return the user (name) that generated this process
std::string Process::User() 
{ 
    return user_ ;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() 
{ 
    return upTime_; 
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }

void Process::getUser()
{
   user_ = LinuxParser::User(Pid()); 
}

void Process::memUtilization()
{
    std::string ram;
    ram = LinuxParser::Ram(Pid());
    mem_ = (std::to_string(std::stof(ram) * 0.001));
}

void Process::GetUpTime()
{
    upTime_ = LinuxParser::UpTime(Pid());
}