#include <numeric>

#include "processor.h"
#include "linux_parser.h"


float Processor::Utilization() {
  auto times = LinuxParser::CpuUtilization();
  float idle_time = times[3];
  float total_time = std::accumulate(times.begin(), times.end(), 0.0);

  float idle_time_diff = idle_time - prev_idle_time_;
  float total_time_diff = total_time - prev_total_time_;
  float cpu_utilization = 1.0 - (idle_time_diff / total_time_diff);

  prev_idle_time_ = idle_time;
  prev_total_time_ = total_time;

  return cpu_utilization;
}