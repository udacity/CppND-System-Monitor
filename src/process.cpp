#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "../include/process.h"
#include "../include/linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization in percent
float Process::CpuUtilization() {
  vector<std::string> stat_values = LinuxParser::CpuUtilization(pid_);
  unsigned long long int total_time = 0;
  float seconds = 0;
  float cpu_usage = 0;
  float hertz = sysconf(_SC_CLK_TCK);
  PidStat stats;

  // Calculate cpu usage per:
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  stats.utime = stoi(stat_values[13]);
  stats.stime = stoi(stat_values[14]);
  stats.cutime = stoi(stat_values[15]);
  stats.cstime = stoi(stat_values[16]);
  stats.starttime = stoi(stat_values[21]);
  total_time = stats.utime + stats.stime;
  total_time = total_time + stats.cutime + stats.cstime;
  seconds = stats.utime - (stats.starttime / hertz);
  cpu_usage = 100 * ((total_time / hertz) / seconds);

  return cpu_usage;
  }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }