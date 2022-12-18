#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid): pid_(pid) {};
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization() const;
  std::string Ram() const;
  long int UpTime();
  bool operator<(Process const& a) const;

 private:
  int pid_;
  std::string command_;
  std::string user_name_ ;
  long up_time_start_{-1};
  long up_time_update_t_;  //local time when we read up time from the file
};

#endif