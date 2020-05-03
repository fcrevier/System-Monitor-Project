#include "processor.h"
#include "linux_parser.h"
#include <vector>

Processor::Processor(int bufferLength) : bufferLength_(bufferLength), totalJiffiesBuffer_(bufferLength_), idleJiffiesBuffer_(bufferLength){
  long idleJiffies = LinuxParser::IdleJiffies();
  long totalJiffies = LinuxParser::Jiffies();

  // init buffers
  for(auto& elem : totalJiffiesBuffer_) {
    elem = totalJiffies;
  }
  for(auto& elem : idleJiffiesBuffer_) {
    elem = idleJiffies;
  }
}

float Processor::Utilization() { 
  long total = LinuxParser::Jiffies();
  long idle = LinuxParser::IdleJiffies();
  
  long prevIdle = idleJiffiesBuffer_[bufferIndex_];
  long prevTotal = totalJiffiesBuffer_[bufferIndex_];
  long dIdle = idle - prevIdle;
  long dTotal = total  - prevTotal;
  long dActive = dTotal - dIdle;
  float cpuUtil;

  // Write to buffer
  idleJiffiesBuffer_[bufferIndex_] = idle;
  totalJiffiesBuffer_[bufferIndex_] = total;

  cpuUtil = (dActive > 0 && dTotal > 0) ? float(dActive)/float(dTotal) : 0.;
  if (bufferIndex_ == bufferLength_-1) { 
    bufferIndex_= 0;
  }
  else {
    bufferIndex_++;
  }
  return cpuUtil;
}