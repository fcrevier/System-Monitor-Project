#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <vector>

class Processor {
 public:
  Processor(int bufferLength);
  float Utilization(); 

 private:
  int bufferLength_;
  int bufferIndex_ = 0;
  std::vector<long> totalJiffiesBuffer_;
  std::vector<long> idleJiffiesBuffer_;
};

#endif