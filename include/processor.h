#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization(); 

 private:
  long int totalJiffies_;
  long int idleJiffies_;
};

#endif