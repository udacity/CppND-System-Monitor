#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  Processor();
  long prev_idle;
  long prev_total;
  // TODO: Declare any necessary private members
 private:
};

#endif