#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  void setCpuUtil();
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(const Process & a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
 private:
  int id_;
  //long int timeStamp_;
  //long int activeTime_;
  float cpuUtil_;
  std::string user_;
  std::string command_;
};

#endif