#include "processor.h"
#include "linux_parser.h"

Processor::Processor() {
   prevTotal_ = LinuxParser::Jiffies();
   prevActive_ = LinuxParser::ActiveJiffies();
}

// Return the aggregate CPU utilization
float Processor::Utilization() { 
   long totalJiffies = LinuxParser::Jiffies();
   long activeJiffies = LinuxParser::ActiveJiffies();
   float cpu_ = float(activeJiffies - prevActive_) / (totalJiffies - prevTotal_);
   prevTotal_ = totalJiffies; 
   prevActive_ = activeJiffies;
   return cpu_;
}