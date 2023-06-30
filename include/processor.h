#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
public:
  /**
   * @brief This returns the processor utilization (0->1)
   *
   * @return processor utilization (0->1)
   */
  float Utilization();

private:
  float prev_tot_jiffies_{0};
  float prev_act_jiffies_{0};
};

#endif