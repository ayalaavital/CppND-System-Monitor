#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>

#include "process.h"
#include "linux_parser.h"

#define GB_TO_MB 0.001
#define RAM_DEFAULT 0

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
string Process::Ram() const {
  string s_ram = LinuxParser::Ram(pid_);
  int i_ram = RAM_DEFAULT;
  if (!s_ram.empty() && std::all_of(s_ram.begin(), s_ram.end(), isdigit)) {
    i_ram = stoi(s_ram) * GB_TO_MB;
  }
  return to_string(i_ram);
}

// Return the user (name) that generated this process
string Process::User() {

  if (user_name_ == ""){
    user_name_ = LinuxParser::User(pid_);
  }
  return user_name_;
}

// Return the age of this process (in seconds)
long int Process::UpTime() {

  using namespace std::chrono;
  auto now = system_clock::now();
  auto now_s = time_point_cast<seconds>(now);
  auto value = now_s.time_since_epoch();
  long duration = value.count();

  if (up_time_start_ < 0){
    up_time_start_ = LinuxParser::UpTime(pid_);
    up_time_update_t_ = duration;
    return up_time_start_;
  }
  long int up_time = up_time_start_ + duration - up_time_update_t_;
  return up_time;
}


bool Process::operator<(Process const& a) const {
  return CpuUtilization() > a.CpuUtilization();
}