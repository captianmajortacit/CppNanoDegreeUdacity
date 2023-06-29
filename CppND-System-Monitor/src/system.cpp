#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#ifndef SYSTEM_H
  #include "system.h"
#endif

#ifndef SYSTEM_PARSER_H
  #include "linux_parser.h"
#endif

using std::set;
using std::size_t;
using std::string;
using std::vector;

/*You need to complete the mentioned TODOs in order to satisfy the rubric criteria "The student will be able to extract and display basic data about the system."

You need to properly format the uptime. Refer to the comments mentioned in format. cpp for formatting the uptime.*/

// DONE: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// DONE: Return a container composed of the system's processes
vector<Process>& System::Processes() { 
  vector<int> loc_pids = LinuxParser::Pids();
  set<int> uniquePID; //to sort out unique PIDs
  float loc_PID_cpuUtilization;
  long loc_PID_Uptime;
  string loc_PID_User;
  string loc_PID_RAM;
  for(auto iter:processes_){
    uniquePID.insert(iter.Pid());
  }

  for(auto iter:loc_pids)
  {
    if(uniquePID.find(iter) == uniquePID.end()){
      
      processes_.emplace_back(iter);
    }
  }
  int  i = 0;
  for(auto iter:processes_)
  {
    loc_PID_User = iter.User();
    loc_PID_Uptime = iter.UpTime();
    loc_PID_RAM = iter.Ram();
    loc_PID_cpuUtilization = iter.CpuUtilization();
    if( (loc_PID_User           == INVALID_USER) ||
        (loc_PID_Uptime         == INVALID_UPTIME_VALUE) ||
        (loc_PID_RAM            == INVALID_RAM) ||
        (loc_PID_cpuUtilization == INVALID_CPU_UTILIZATION) )
    {
      processes_.erase(processes_.begin() + i);
      i = (i>0) ? i-1 : 0;
    }
    else
    {
      i++;
    }  
  }
  std::sort(processes_.begin(), processes_.end(), std::less<Process>());
  return processes_; }

// DONE: Return the system's kernel identifier (string)
std::string System::Kernel() {return LinuxParser::Kernel();} 

// DONE: Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// DONE: Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem();}

// DONE: Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// DONE: Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// DONE: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
