#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

#define INVALID_UPTIME_VALUE     -1         // implausible time value
#define INVALID_VALUE            -1         // implausible memory value
#define INVALID_JIFFIE_VALUE     -1         // implausible jiffy value
#define INVALID_CPU_UTILIZATION 999         // implausible percentage value
#define INVALID_USER            "NOT FOUND" // implausible user value
#define INVALID_RAM             "NOT FOUND" // implausible user value

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

// Token added on suggestion from the reviewer
const std::string filterProcesses("processes");
const std::string filterRunningProcesses("procs_running");
const std::string filterMemTotalString("MemTotal:");
const std::string filterMemFreeString("MemFree:");
const std::string filterCpu("cpu");
const std::string filterUID("Uid:");
const std::string filterProcMem("VmRSS:"); // The string is changed from VmSize to VmRSS as suggested by the reviewer
//"VmSize is the sum of all the virtual memory as you can see on the manpages also.
//Whereas when you use VmRSS then it gives the exact physical memory being used as a part of Physical RAM.
// So it is recommended to replace the string VmSize with VmRSS"

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
  kGuestNice_,
  CPUStates_MAX_ =10
};
std::vector<long> CpuUtilization();
long Jiffies(const CPUStates arg_CPUState);
long ActiveJiffies();
long ActiveJiffies(int pid);
long IdleJiffies();

// Processes
std::string Command(int pid);
std::string Ram(int pid);
std::string Uid(int pid);
std::string User(int pid);
long int UpTime(int pid);

};  // namespace LinuxParser

#endif