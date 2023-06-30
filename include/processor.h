#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
  float Utilization();

private:
  float prev_tot_jiffies_{0};
  float prev_act_jiffies_{0};
};

#endif