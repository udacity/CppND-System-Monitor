#ifndef PROCESSOR_H
#define PROCESSOR_H

/*
from stackoverflow link which was provided
// Guest time is already accounted in usertime
usertime = usertime - guest;                             # As you see here, it subtracts guest from user time
nicetime = nicetime - guestnice;                         # and guest_nice from nice time
// Fields existing on kernels >= 2.6
// (and RHEL's patched kernel 2.4...)
unsigned long long int idlealltime = idletime + ioWait;  # ioWait is added in the idleTime
unsigned long long int systemalltime = systemtime + irq + softIrq;
unsigned long long int virtalltime = guest + guestnice;
unsigned long long int totaltime = usertime + nicetime + systemalltime + idlealltime + steal + virtalltime;

PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald
*/
class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  //setters
  void PrevUser(float user) { prev_user_ = user; };
  void PrevNice(float nice) { prev_nice_ = nice; };
  void PrevSystem(float system) { prev_system_ = system; };
  void PrevIdle(float idle) { prev_idle_ = idle; };
  void PrevIowait(float iowait) { prev_iowait_ = iowait; };
  void PrevIRQ(float irq) { prev_irq_ = irq; };
  void PrevSoftIRQ(float softirq) { prev_softirq_ = softirq; };
  void PrevSteal(float steal) { prev_steal_ = steal; };

  // TODO: Declare any necessary private members
  private:
    float prev_user_;
    float prev_nice_;
    float prev_system_;
    float prev_idle_;
    float prev_iowait_;
    float prev_irq_;
    float prev_softirq_;
    float prev_steal_;
};
  

#endif