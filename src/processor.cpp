#include "processor.h"

#include "linux_parser.h"

float Processor::Utilization() { 
    long totalJiffies = LinuxParser::Jiffies();
    long idleJiffies = LinuxParser::IdleJiffies();

    if (previousTotal == 0) {
        previousTotal = totalJiffies;
    }

    if (previousIdle == 0) {
        previousIdle = idleJiffies;
    }

    long deltaTotalJiffies = totalJiffies - previousTotal;

    if (deltaTotalJiffies == 0) {
        return 0.0;
    }

    long deltaIdleJiffies = idleJiffies - previousIdle;
    long deltaActiveJiffies = deltaTotalJiffies - deltaIdleJiffies;

    float utilization = float(deltaActiveJiffies) / float(deltaTotalJiffies);

    previousTotal = totalJiffies;
    previousIdle = idleJiffies;

    return utilization;
}