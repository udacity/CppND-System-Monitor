#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() {
    return pid_;
}

void Process::Pid(int p) {
    pid_ = p;
}

float Process::CpuUtilization() {
    return LinuxParser::CpuUtilization(pid_);
}

string Process::Command() {
    return LinuxParser::Command(pid_);
}

string Process::Ram() {
    auto ramString = LinuxParser::Ram(pid_);

    if (ramString != "") {
        auto ramFloat = std::stof(ramString);
        ramFloat /= 1000.0f;
        std::stringstream stream;
        stream << std::fixed << std::setprecision(2) << ramFloat;
        ramString = stream.str();
    }

    return ramString;
}

string Process::User() {
    return LinuxParser::User(pid_);
}

long int Process::UpTime() {
    return LinuxParser::UpTime(pid_);
}

bool Process::operator<(Process const& a) const {
    return LinuxParser::CpuUtilization(pid_) < LinuxParser::CpuUtilization(a.pid_);
}

bool Process::operator==(Process const& a) const {
    return pid_ == a.pid_;
}