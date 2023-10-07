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

Process::Process(int pid) : pid_{pid} {
  keepAlive_ = true;
  memUsage_ = "";
  cpuUsage_ = 0;
  currentActiveJiffies_ = 0;
  currentActiveJiffies_ = 0;
  username_ = "";
  userInitialized_ = false;
  command_ = "";
  commandInitialized_ = false;
  upTime_ = 0;
}

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
  if ((int)(cpuUsage_ * 1000) <= 0) {
    return 0;
  } else {
    return cpuUsage_;
  }
}

// DONE: Return the command that generated this process
string Process::Command() {
  if (!commandInitialized_) {
    commandInitialized_ = true;
    command_ = LinuxParser::Command(pid_);
  }
  return (command_);
}

// DONE: Return this process's memory utilization
string Process::Ram() { return (memUsage_); }

// DONE: Return the user (name) that generated this process
string Process::User() {
  if (!userInitialized_) {
    userInitialized_ = true;
    username_ = LinuxParser::User(pid_);
  }
  return username_;
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return upTime_; }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
  return ((int)(a.cpuUsage_ * 1000) < (int)(cpuUsage_ * 1000));
}
bool Process::operator==(Process const& a) const { return (pid_ == a.pid_); }

bool Process::KeepAlive() { return keepAlive_; }
void Process::KeepAlive(bool alive) { keepAlive_ = alive; }
void Process::updateCpuUsage(void) {
  float previousActiveJiffies = currentActiveJiffies_;
  float previousTotalJiffies = currentTotalJiffies_;
  currentActiveJiffies_ = LinuxParser::ActiveJiffies(pid_);
  currentTotalJiffies_ = LinuxParser::Jiffies();
  cpuUsage_ = (((currentActiveJiffies_ - previousActiveJiffies) /
                (currentTotalJiffies_ - previousTotalJiffies)));
}

void Process::updateMemUsage(void) {
  if (memUsage_ == "") {
    string vMem = LinuxParser::Ram(pid_);
    memUsage_ = std::to_string((std::stol(vMem) / 1000));
  }
}

void Process::updateUpTime() {
  long rawUpTime = LinuxParser::UpTime(pid_);
  upTime_ = rawUpTime / sysconf(_SC_CLK_TCK);
}

void Process::UpdateProcess() {
  updateUpTime();
  updateCpuUsage();
  updateMemUsage();
}