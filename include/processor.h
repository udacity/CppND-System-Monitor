#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor(); // initialize cpu values
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
   long prevTotal_;
   long prevActive_;
};

#endif