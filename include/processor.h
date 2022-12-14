#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
  float prev_idle_time_{0};
  float prev_total_time_{0};
};

#endif