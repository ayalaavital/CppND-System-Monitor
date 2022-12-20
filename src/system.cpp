#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <ctime>
#include <chrono>

#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;


Processor& System::Cpu() { return cpu_; }


vector<Process>& System::Processes() {

  vector<int> pids(LinuxParser::Pids());
  set<int> old_pids;
  vector<int>::iterator it;
  vector<int> to_erase{};

  for (Process& process: processes_){
    int pid = process.Pid();
    it = find(pids.begin(), pids.end(), pid);
    if (it == pids.end()){   // process terminate
      to_erase.emplace_back(pid);
    } else {
      old_pids.insert(pid);
    }
  }

  for (int pid: to_erase) {
    auto iter = std::find_if(processes_.begin(), processes_.end(), [pid](auto process) { return process.Pid() == pid; });
    processes_.erase(iter);
  }

  for(auto pid: pids){
    if(old_pids.find(pid) == old_pids.end()){
      processes_.emplace_back(pid);
    }
  }

  std::sort(processes_.begin(), processes_.end());
  return processes_;
}


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

  using namespace std::chrono;
  auto now = system_clock::now();
  auto now_s = time_point_cast<seconds>(now);
  auto value = now_s.time_since_epoch();
  long duration = value.count();

  if (up_time_start_ < 0){
    up_time_start_ = LinuxParser::UpTime();
    up_time_update_t_ = duration;
    return up_time_start_;
  }
  long int up_time = up_time_start_ + duration - up_time_update_t_;
  return up_time;
}