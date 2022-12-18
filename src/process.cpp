#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <ctime>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {
  return LinuxParser::ActiveJiffies(pid_) / 100.0;
}

// Return the command that generated this process
string Process::Command() {

  if (command_ == ""){
    command_ = LinuxParser::Command(pid_);
  }
  return command_;
}

// Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(pid_); }

// Return the user (name) that generated this process
string Process::User() {

  if (user_name_ == ""){
    user_name_ = LinuxParser::User(pid_);
  }
  return user_name_;
}

// Return the age of this process (in seconds)
long int Process::UpTime() {
  std::time_t t = std::time(nullptr);
  std::localtime(&t);
  if (up_time_start_ < 0){
    up_time_start_ = LinuxParser::UpTime(pid_);
    up_time_update_t_ = t;
    return up_time_start_;
  }
  long int up_time = up_time_start_ + t - up_time_update_t_;
  return up_time;
}


bool Process::operator<(Process const& a) const {
  return CpuUtilization() > a.CpuUtilization();
}