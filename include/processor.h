#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  float PrevIdle, Idle, PrevNonIdle, NonIdle, PrevTotal, Total, totald, idled;

  // TODO: Declare any necessary private members
 private:
  float user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
};

#endif