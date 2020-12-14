#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <iterator>
#include <vector>

#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

//defining the constructor
//hz should be sysconf(_SC_CLK_TCK)
Process::Process(int pid, long hz) : pid_(pid), hz_(hz) {
  string line;
  std::ifstream filestream(LinuxParser::kProcDirectory + to_string(pid) +
                       LinuxParser::kStatFilename);

  if(filestream.is_open()){
  std::getline(filestream, line);
  std::istringstream linestream(line);
  std::istream_iterator<string> beg(linestream), end; // splits the string by white-space and returns all tokens in a vector. 
  std::vector<string> time(beg, end);
  utime_ = stof(time[13]);
  stime_ = stof(time[14]);
  cutime_ = stof(time[15]);
  cstime_ = stof(time[16]);
  starttime_ = stof(time[21]);
  }
}
// TODO: Return this process's ID
int Process::Pid() { 
  return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
  long uptime; 
  float totaltime;  
  float seconds; 
  float cpuutilization;
  
  uptime = LinuxParser::UpTime();
  totaltime = utime_ + stime_ + cutime_ + cstime_;
  seconds =  uptime - (starttime_ / hz_);
  cpuutilization = (totaltime / hz_) / seconds; // I have read a mentor answer that i don't multiply by 100 because it will be done in the ncurses_display

  return cpuutilization;
  }

// TODO: Return the command that generated this process
string Process::Command() { 
  return LinuxParser::Command(pid_); 
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
  return LinuxParser::Ram(pid_);
  }

// TODO: Return the user (name) that generated this process
string Process::User() { 
  string uid = LinuxParser::Uid(pid_);
  int UID = std::stoi(uid);
  return LinuxParser::User(UID);  
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
  long uptime; 
  long seconds; 
  uptime = LinuxParser::UpTime();
  seconds =  uptime - (starttime_ / hz_);
  return seconds;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process & a) {
  return (CpuUtilization() < a.CpuUtilization());
}