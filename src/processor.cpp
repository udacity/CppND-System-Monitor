#include "processor.h"
#include "linux_parser.h"

// Return the aggregate CPU utilization
float Processor::Utilization() { 
   long totalJiffies = LinuxParser::Jiffies();
   long activeJiffies = LinuxParser::ActiveJiffies();
   
   return float(activeJiffies) / totalJiffies;        
}