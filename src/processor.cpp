#include "processor.h"
#include "linux_parser.h"


float Processor::Utilization() { 

  long idle_time = LinuxParser::IdleJiffies();
  long total_time = LinuxParser::Jiffies();

  long idle_time_diff = idle_time - prev_idle_time_;
  long total_time_diff = total_time - prev_total_time_;
  float cpu_utilization = 1.0 - float(idle_time_diff) / float(total_time_diff);

  prev_idle_time_ = idle_time;
  prev_total_time_ = total_time;

  return cpu_utilization;
}