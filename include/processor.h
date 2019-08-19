#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  long previousTotal{0};
  long previousIdle{0};
};

#endif