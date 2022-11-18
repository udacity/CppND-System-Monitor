#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid), 
                            user_(LinuxParser::User(pid)),
                            command_(LinuxParser::Command(pid)) {

}
// Done: Return this process's ID
int Process::Pid() { return pid_; }

// Done: Return this process's CPU utilization
// totaltime = cu+cs
// duration = uptime - (starttime / herz)
float Process::CpuUtilization() { 
    
    float util = 0.0;
    long total_time = LinuxParser::ActiveJiffies(pid_);
    long duration_sec = LinuxParser::UpTime() - LinuxParser::UpTime(pid_); //elapsed time since process started
    util = ((float)total_time / sysconf(_SC_CLK_TCK) ) / float(duration_sec);
    cur_cpu_util_ = util;
    return util; 
}

// Done: Return the command that generated this process
string Process::Command() { return command_; }

// Done: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// Done: Return the user (name) that generated this process
string Process::User() { 
    return user_; 
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    return LinuxParser::UpTime(this->pid_);
}

// // TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return this->cur_cpu_util_ > a.cur_cpu_util_ ;
}

bool Process::isValid() {
    if (user_.size() > 0 && command_.size() > 0) {
        return true;
    }
    return false;
}