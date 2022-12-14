#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();


 private:
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  std::string os_{" "};
  std::string kernel_{" "};
  long up_time_start_{-1};
  long up_time_update_t_;  //local time when we read up time from the file
};

#endif