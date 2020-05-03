#include "processor.h"
#include "linux_parser.h"

Processor::Processor() {
  idleJiffies_ = LinuxParser::IdleJiffies();
  totalJiffies_ = LinuxParser::Jiffies();
}

float Processor::Utilization() { 
  long currTotal = LinuxParser::Jiffies();
  long currIdle = LinuxParser::IdleJiffies();
  long dTotal =  currTotal - totalJiffies_;
  long dIdle  = currIdle - idleJiffies_;
  float output; // for debug
  totalJiffies_ = currTotal;
  idleJiffies_ = currIdle;
  
  if (dTotal > dIdle && dTotal!= 0) { output = float(dTotal-dIdle)/float(dTotal); }
  else { output = 0.; }
  return output;
}