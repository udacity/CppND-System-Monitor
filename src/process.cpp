#include "process.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Construct a process from a given process id
Process::Process(int pid)
  : process_id_(pid)
  {
    Command();
    Ram();
    User();
    UpTime();
  }

// DONE: Return this process's ID
int Process::Pid() { return process_id_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
  return cpu_usage_; }

// DONE: Return the command that generated this process
string Process::Command() { 
  command_ = LinuxParser::Command(process_id_);
  return command_; }

// DONE: Return this process's memory utilization
string Process::Ram() { 
  ram_ = LinuxParser::Ram(process_id_);
  return ram_; }

// DONE: Return the user (name) that generated this process
string Process::User() { 
  user_ = LinuxParser::User(Pid());
  return user_; 
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
  return cpu_usage_ < a.cpu_usage_;
}