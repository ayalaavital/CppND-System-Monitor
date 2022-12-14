#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <ctime>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }


std::string System::Kernel() {
  if (kernel_ == " "){
    kernel_ = LinuxParser::Kernel();
  }
  return kernel_;
}


float System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}


std::string System::OperatingSystem() {
  if (os_ == " "){
    os_ = LinuxParser::OperatingSystem();
  }
  return os_;
}


int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}


int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
}


long int System::UpTime() {
  std::time_t t = std::time(nullptr);
  std::localtime(&t);
  if (up_time_start_ < 0){
    up_time_start_ = LinuxParser::UpTime();
    up_time_update_t_ = t;
    return up_time_start_;
  }
  long int up_time = up_time_start_ + t - up_time_update_t_;
  return up_time;
}