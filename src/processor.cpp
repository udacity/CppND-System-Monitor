#include "processor.h"
#include "linux_parser.h"


Processor::Processor(){
    this->prev_idle = 0;
    this->prev_total = 0;
}
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    long idle = LinuxParser::IdleJiffies();
    long total = LinuxParser::Jiffies();
    long diff_idle = idle - this->prev_idle;
    long diff_total = total - this->prev_total;
    float diff_usage = (10*(diff_total-diff_idle)/diff_total)/(float)10;
    this->prev_idle = idle;
    this->prev_total = total;
    return diff_usage;
    // return (float) LinuxParser::ActiveJiffies() / (float) LinuxParser::Jiffies(); 
}