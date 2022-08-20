#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
    Processor();
    float Utilization();  // See src/processor.cpp

  // Declare any necessary private members
 private:
    long totalJiffies;
    long idleJiffies;
};

#endif