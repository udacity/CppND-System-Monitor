#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::vector;

Processor::Processor() : totalJiffies(0), idleJiffies(0) {};

// Return the aggregate CPU utilization
float Processor::Utilization() {
    vector<long> cpuStats = LinuxParser::CpuUtilization();

    // https://stackoverflow.com/a/23376195/16112875

    // add a 1 to avoid zero division
    long updatedTotalJiffies = LinuxParser::Jiffies();
    long updatedIdleJiffies = LinuxParser::IdleJiffies();

    float deltaTotal = updatedTotalJiffies - totalJiffies;
    float deltaIdle = updatedIdleJiffies - idleJiffies;

    totalJiffies = updatedTotalJiffies;
    idleJiffies = updatedIdleJiffies;

    // return fraction, % calculated in ncurses_display.cpp
    return (deltaTotal - deltaIdle) / deltaTotal;
}