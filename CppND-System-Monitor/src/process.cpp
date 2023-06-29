#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#ifndef PROCESS_H
  #include "process.h"
#endif

#ifndef SYSTEM_PARSER_H
  #include "linux_parser.h"
#endif

using std::string;
using std::to_string;
using std::vector;

Process::Process(const int arg_processID){
    processID_= arg_processID;
    cpuUtilization_ = 0;
}

// DONE: Return this process's ID
int Process::Pid() { return processID_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
    const long loc_UpTimePID = LinuxParser::UpTime(processID_);
    const long loc_ActiveJiffies = LinuxParser::ActiveJiffies(processID_);
    const long loc_totalTimeInSecondsSinceStartup = LinuxParser::UpTime() - loc_UpTimePID;
    if((loc_totalTimeInSecondsSinceStartup == 0) || (loc_UpTimePID == INVALID_UPTIME_VALUE) || (loc_ActiveJiffies ==  INVALID_VALUE))
    {
      cpuUtilization_ = INVALID_CPU_UTILIZATION;
    }
    else
    {
      cpuUtilization_ = (1.0 * loc_ActiveJiffies )/ loc_totalTimeInSecondsSinceStartup;
    }
        
    return cpuUtilization_; }

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(processID_); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(processID_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(processID_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
  const long int loc_UptimePID = LinuxParser::UpTime() - LinuxParser::UpTime(processID_);
  return ((loc_UptimePID < 0) ? 0 : loc_UptimePID); // checking if the difference is negative as time cannot be negative
 }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const {
     return a.cpuUtilization_ < this->cpuUtilization_; }