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

Process::Process(int pid) : id_(pid) {
  //activeTime_ = LinuxParser::ActiveJiffies(id_)/sysconf(_SC_CLK_TCK);
  //timeStamp_ = LinuxParser::UpTime();
  user_ = LinuxParser::User(id_);
  command_ = LinuxParser::Command(id_);
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
  long uptime = LinuxParser::UpTime(id_);
  //long dActiveTime = currActiveTime - activeTime_;
  //long dTime =  currTime - timeStamp_;
  float output = 0.; // for debug;

  /*
  activeTime_ = currActiveTime;
  timeStamp_ = currTime;
  if(dTime !=0 && dActiveTime > 0) { 
    output = float(dActiveTime)/float(dTime); 
  }
  else { output = 0.; }
  */
  if (uptime > 0) {
    output = float(activeTime)/float(uptime);
  }
  cpuUtil_ = output;
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