#ifndef PROCESSOR_H
  #include "processor.h"
#endif

#ifndef SYSTEM_PARSER_H
  #include "linux_parser.h"
#endif

// DONE: Return the aggregate CPU utilization
float Processor::Utilization() { 
  static long loc_lastTotal_= LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
  static long loc_lastIdle_ = LinuxParser::IdleJiffies();
  const long loc_currTotal = LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();
  const long loc_deltaIdle = LinuxParser::IdleJiffies() - loc_lastIdle_;
  const long loc_deltaTotal = loc_currTotal - loc_lastTotal_;

  loc_lastTotal_ = loc_currTotal;
  loc_lastIdle_ = LinuxParser::IdleJiffies();

  const float loc_retVal = (loc_deltaTotal == 0) ? 0.0 : (loc_deltaTotal - loc_deltaIdle)*1.0/loc_deltaTotal;

  return loc_retVal;
 }