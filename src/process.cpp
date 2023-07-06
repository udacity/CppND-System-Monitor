#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

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
  string stats_string_concat = "";
  PidStat stats = {};

  // Calculate cpu usage per
  // https://stackoverflow.com/questions/16726779/how-do-i-get-the-total-cpu-usage-of-an-application-from-proc-pid-stat/16736599#16736599
  for (long unsigned int i = 0; i < stats_string.size(); i++) {
    std::istringstream linestream(stats_string[i]);
    if ( i == 13 )
      linestream >> stats.utime;
    if ( i == 14 )
      linestream >> stats.stime;
    if ( i == 15 )
      linestream >> stats.cutime;
    if ( i == 16 )
      linestream >> stats.cstime;
    if ( i == 21 )
      linestream >> stats.starttime;
  }
  total_time = stats.utime + stats.stime;
  // total_time = total_time + stats.cutime + stats.cstime;  // Include if child processes should be counted too
  seconds = stats.utime - (stats.starttime / hertz);
  cpu_ = 100 * ((total_time / hertz) / seconds);
}

// Return this process's CPU utilization in percent
float Process::CpuUtilization() { return cpu_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return uptime_; }

// Return the command that generated this process
string Process::Command() { return command_; }

// Return the user (name) that generated this process
string Process::User() { return username_; }

// Return this process's memory utilization
string Process::Ram() { return to_string(static_cast<int>(ram_)); }

// Set user id and username
void Process::User(vector<string> statuses) {
  string uid = {};
  string key = {};

  // Find Uid and set class data
  for ( auto status : statuses ) {
    std::istringstream linestream(status);
    while (linestream >> key >> uid) {
      if (key == "Uid")
        uid_ = uid;
    }
  }

  // Use uid to identify username and set class data
  username_ = LinuxParser::ReadUser(uid_);
}

// Set Ram
void Process::Ram(vector<string> statuses) {
  int ram = {};
  string key = {};
  float kb_per_mb = 1024.0;


  for ( auto status : statuses ) {
    std::istringstream linestream(status);
    while (linestream >> key >> ram) {
      if (key == "VmSize")
        ram_ = ram / kb_per_mb;
    }
  }
}

// Set command
void Process::Command(string commands) {
  string command = {};

  // Extract first entry and set to class data
  std::istringstream linestream(commands);
  linestream >> command;
  command_ = command;
}

// Set Uptime
void Process::UpTime(vector<string> stats) {
  int uptime_index = 13;  // Uptime in ticks.
  std::istringstream linestream(stats[uptime_index]);
  long uptime;
  linestream >> uptime;
  uptime_ = uptime / sysconf(_SC_CLK_TCK);  // Convert ticks to seconds.
}

// Overload the "less than" comparison operator for Process objects
bool Process::operator>(Process const& a) const {
  return cpu_ > a.cpu_;
}