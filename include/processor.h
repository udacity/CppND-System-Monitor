#ifndef PROCESSOR_H
#define PROCESSOR_H

struct Status {
  unsigned long long int user = 0;
  unsigned long long int nice = 0;
  unsigned long long int system = 0;
  unsigned long long int idle = 0;
  unsigned long long int iowait = 0;
  unsigned long long int irq = 0;
  unsigned long long int softirq = 0;
  unsigned long long int steal = 0;
  unsigned long long int guest = 0;
  unsigned long long int guestnice = 0;
};

class Processor {
 public:
  Processor() {
    ReadStatus();
  }
  float Utilization();

 private:
  Status current_;
  Status previous_;
  void ReadStatus();
};

#endif