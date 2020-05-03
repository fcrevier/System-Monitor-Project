#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stol;
using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, temp;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> temp >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string mem_free, mem_tot;
  string line;
  string temp;
  
  // Parse first 2 lines
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  std::getline(stream, line);
  std::istringstream linestream1(line);
  linestream1 >> temp >> mem_tot;
  std::getline(stream, line);
  std::istringstream linestream2(line);
  linestream2 >> temp >> mem_free;
  
  // Compute used memory ratio
  float mem_tot_num = stof(mem_tot);
  float mem_free_num = stof(mem_free);
  float mem_util = (mem_tot_num - mem_free_num)/mem_tot_num;
  return mem_util;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  string uptime;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  std::getline(stream, line);
  std::istringstream linestream(line);
  linestream >> uptime;
  return stol(uptime);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  string line;
  string temp;
  long total = 0; // sum of jiffies
  
  // Get first line (cpu data)
  std::ifstream stream(kProcDirectory + kStatFilename);
  std::getline(stream, line);
  
  // Add 8 first jiffy values (not guest)
  std::istringstream linestream(line);
  linestream >> temp;
  for (int i = 0; i < 8; i++) {
    linestream >> temp;
    total += stol(temp);
  }
  return total;
}

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  long active = 0;
  string line;
  string temp;
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= 17; i++) {
      linestream >> temp;
      if (i >= 14) { active += stol(temp); }
    }
  }
    return active;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  return LinuxParser::Jiffies() - LinuxParser::IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line;
  string temp;
  long idle = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  // Get first line (cpu data)
  std::getline(stream, line);
  
  // Get idle and iowait jiffy values (not guest)
  std::istringstream linestream(line);
  linestream >> temp;
  for (int i = 0; i <= 4; i++) {
    linestream >> temp;
    if (i >= 3) { 
      idle += stol(temp); 
    }
  }
  return idle;
}

// TODO: Read and return CPU utilization
// NOT USED.
vector<string> LinuxParser::CpuUtilization() { return {}; }
//long LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string temp;
  int tot;
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  // Reach total processes line
  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> temp;
    if (temp == "processes") {
      linestream >> temp;
      tot = stoi(temp);
      return tot;
    }
  }
  return tot;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string temp;
  int run;
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  // Reach total processes line
  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> temp;
    if (temp == "procs_running") {
      linestream >> temp;
      run = stoi(temp);
      return run;
    }
  }
  return run;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  string cmd;
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()){
  	std::getline(stream, line);
  }
  return line;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  string line;
  string temp;
  string ram;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  
  // Reach total processes line
  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> temp;
    if (temp == "VmSize:") {
      linestream >> temp;
      ram = temp;
      return ram;
    }
  }
  return ram;
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line;
  string temp;
  string uid;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatusFilename);
  
  // Reach total processes line
  while (std::getline(stream, line)) {
    std::istringstream linestream(line);
    linestream >> temp;
    if (temp == "Uid:") {
      linestream >> temp;
      uid = temp;
      return uid;
    }
  }
  return uid;
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string line;
  string user;
  string tempUser;
  string temp;
  string id;
  string uid = LinuxParser::Uid(pid);
  
  std::ifstream stream( kPasswordPath );
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> tempUser >> temp >> id;
      if (id == uid) { 
        user = tempUser;
        return user; 
      }
    }
  }
  return user;
}
        
// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  int clock_id = 22;
  long clock = 0;
  string line;
  string temp;
  
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for (int i = 1; i <= clock_id; i++) {
      linestream >> temp;
    }
    clock = stol(temp);
  }
    return LinuxParser::UpTime() - clock/sysconf(_SC_CLK_TCK);
}
