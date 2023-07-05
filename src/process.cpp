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

// Set Cpu utilization
void Process::CpuUtilization(std::vector<std::string> stats_string) {
  unsigned long long int total_time = 0;
  float seconds = 0;
  float hertz = sysconf(_SC_CLK_TCK);
  PidStat stats = {};

  // Calculate cpu usage per
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  stats.utime = stoi(stats_string[13]);
  stats.stime = stoi(stats_string[14]);
  stats.cutime = stoi(stats_string[15]);
  stats.cstime = stoi(stats_string[16]);
  stats.starttime = stoi(stats_string[21]);
  total_time = stats.utime + stats.stime;
  total_time = total_time + stats.cutime + stats.cstime;
  seconds = stats.utime - (stats.starttime / hertz);
  cpu_ = 100 * ((total_time / hertz) / seconds);
}

// Return this process's CPU utilization in percent
float Process::CpuUtilization() { return cpu_; }

// Set Uptime
void Process::UpTime(vector<string> stats) {
  int uptime_index = 22;  // 22nd item is updtime in ticks
  long uptime = stoi(stats[uptime_index]);
  uptime_ = uptime / sysconf(_SC_CLK_TCK);  // Convert ticks to seconds.
}

// Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
// bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }