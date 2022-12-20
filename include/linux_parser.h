#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {
// Paths
const std::string kProcDirectory{"/proc/"};
const std::string kCmdlineFilename{"/cmdline"};
const std::string kCpuinfoFilename{"/cpuinfo"};
const std::string kStatusFilename{"/status"};
const std::string kStatFilename{"/stat"};
const std::string kUptimeFilename{"/uptime"};
const std::string kMeminfoFilename{"/meminfo"};
const std::string kVersionFilename{"/version"};
const std::string kOSPath{"/etc/os-release"};
const std::string kPasswordPath{"/etc/passwd"};

// System
float MemoryUtilization();
long UpTime();
std::vector<int> Pids();
int TotalProcesses();
int RunningProcesses();
std::string OperatingSystem();
std::string Kernel();

// CPU
enum CPUStates {
  kUser_ = 0,
  kNice_,
  kSystem_,
  kIdle_,
  kIOwait_,
  kIRQ_,
  kSoftIRQ_,
  kSteal_,
  kGuest_,
  kGuestNice_
};
std::vector<std::string> CpuUtilization();
long Jiffies();
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
enum ProcessStat {
  pid_ = 0,
  comm_,
  state_,
  ppid_,
  pgrp_,
  session_,
  ttyNr_,
  tpgid_,
  flags_,
  minflt_,
  cmiinflt_,
  majflt_,
  cmajflt_,
  utime_,
  stime_,
  cutime_,
  cstime_,
  priority_,
  nice_,
  numThreads_,
  itrealvalue_,
  starttime_,
  vsize_,
  rss_,
  rsslim_,
  startcode_,
  endcode_,
  startstack_,
  kstkesp_,
  kstkeip_,
  signal_,
  blocked_,
  sigignore_,
  sigcatch_,
  wchan_,
  nswap_,
  cnswap_,
  exitSignal_,
  processor_,
  rtPriority_,
  policy_,
  delayacctBlkioTicks_,
  guestTime_,
  cguestTime_,
  startData_,
  endData_,
  startBrk_,
  argStart_,
  argEnd_,
  envStart_,
  envEnd_,
  exitCode_
};

std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);
std::vector<std::string> StatParser(int pid);
};  // namespace LinuxParser

#endif