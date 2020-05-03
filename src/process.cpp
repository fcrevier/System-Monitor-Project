#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::stol;

Process::Process(int pid, int bufferLength) : id_(pid), bufferLength_(bufferLength), activeTimeBuffer_(bufferLength), upTimeBuffer_(bufferLength) {
  user_ = LinuxParser::User(id_);
  command_ = LinuxParser::Command(id_);

  // init buffers
  long activeTime = LinuxParser::ActiveJiffies(id_)/sysconf(_SC_CLK_TCK);
  long upTime = LinuxParser::UpTime(id_);
  for(auto& elem : activeTimeBuffer_) {
    elem = activeTime;
  }
  for(auto& elem : upTimeBuffer_) {
    elem = upTime;
  }
  setCpuUtil();
}
// TODO: Return this process's ID
int Process::Pid() { return id_; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
  this->setCpuUtil();
  return cpuUtil_;
}

void Process::setCpuUtil() {
  long activeTime = LinuxParser::ActiveJiffies(id_)/sysconf(_SC_CLK_TCK);
  long upTime = LinuxParser::UpTime(id_);

  long prevActiveTime = activeTimeBuffer_[bufferIndex_];
  long prevUpTime = upTimeBuffer_[bufferIndex_];
  long dActiveTime = activeTime - prevActiveTime;
  long dUpTime = upTime  - prevUpTime;
  bool stop = true;
  // Write to buffer
  activeTimeBuffer_[bufferIndex_] = activeTime;
  upTimeBuffer_[bufferIndex_] = upTime;
  if (dActiveTime > 0){ 
    stop = false;
    }
  cpuUtil_ = (dActiveTime > 0 && dUpTime > 0) ? float(dActiveTime)/float(dUpTime) : 0.;
  if (bufferIndex_ == bufferLength_-1) { 
    bufferIndex_= 0;
  }
  else {
    bufferIndex_++;
  }
}
// TODO: Return the command that generated this process
string Process::Command() { return command_; }

// TODO: Return this process's memory utilization
string Process::Ram() { 
  long ram;
  try {
    ram = stol(LinuxParser::Ram(id_))/1000; 
  }
  catch (const std::invalid_argument& e) {
    ram = 0;
  }
  return to_string(ram);
}

// TODO: Return the user (name) that generated this process
string Process::User() { return user_; }

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(id_); }

// TODO: Overload the "less than" comparison operator for Process objects
bool Process::operator<(const Process &a) const { return this->cpuUtil_ > a.cpuUtil_; }