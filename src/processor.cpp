#include "processor.h"

#include <string>
#include <vector>

#include "linux_parser.h"

bool Processor::IsInitializied_ = false;

Processor::Processor() {
  CurrentTotalTime_ = 0.0;
  CurrentRunTime_ = 0.0;
}
// DONE: Return the aggregate CPU utilization
float Processor::Utilization() {
  LinuxParser::CpuUtilization();
  float PreviousTotalTime_ = CurrentTotalTime_;
  float PreviousRunTime_ = CurrentRunTime_;
  CurrentTotalTime_ = LinuxParser::Jiffies();
  CurrentRunTime_ = LinuxParser::ActiveJiffies();

  if (!IsInitializied_) {
    IsInitializied_ = true;
    return 0.0;
  }
  // TODO: [RH] handle overflow possibility
  return ((CurrentRunTime_ - PreviousRunTime_)) /
         ((CurrentTotalTime_ - PreviousTotalTime_));
}