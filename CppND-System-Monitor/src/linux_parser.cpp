#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#ifndef SYSTEM_PARSER_H
  #include "linux_parser.h"
#endif

//Defines
//position of uptime value in /proc/[PID]/stat
#define PID_UPTIME_POSITION 22
//position of values in /proc/[PID]/stat for calculating Jiffies of [PID]
#define PID_UTIME_POSITION 14

//namespaces
using std::stof;
using std::string;
using std::to_string;
using std::vector;

//Helper functions
/* Declaration of GetLineFromFile(std::string arg_FilePath)*/
std::string GetLineFromFile(std::string arg_FilePath);
// Declaration of GetValueFromFile(std::string arg_FilePath, std::string arg_token
long GetValueFromFile(std::string arg_FilePath, std::string arg_token);


//Helper functions
/* Definition of GetLineFromFile(std::string arg_FilePath)*/
std::string GetLineFromFile(std::string arg_FilePath)
{
  string loc_line;
  std::ifstream loc_stream(arg_FilePath);
  if (loc_stream.is_open())
    std::getline(loc_stream, loc_line);
  return loc_line;
}

//Definition of GetValueFromFile(std::string arg_FilePath, std::string arg_token) 
long GetValueFromFile(std::string arg_FilePath, std::string arg_token){
  string loc_line;
  string loc_key;
  long loc_value = INVALID_VALUE;

  std::ifstream loc_filestream(arg_FilePath);
  if (loc_filestream.is_open()) {
    while (std::getline(loc_filestream, loc_line)) {
      if(loc_line.find(arg_token)!= std::string::npos)
      {
        std::istringstream loc_linestream(loc_line);
        loc_linestream >> loc_key;
        if(!(loc_linestream >> loc_value).fail()){
          break;
        }
        else
        {
          //do nothing
        }        
      }
    }
  }

  return loc_value;
}


// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}


// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  std::istringstream linestream(GetLineFromFile(kProcDirectory + kVersionFilename));
  linestream >> os >> version >> kernel;

  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization(){
  static long loc_lastTotalMemory = 0;
  static long loc_lastFreeMemory = 0;

  const long loc_totalMemoryFromFile = (GetValueFromFile(kProcDirectory + kMeminfoFilename, filterMemTotalString));
  const long loc_FreeMemoryFromFile  = (GetValueFromFile(kProcDirectory + kMeminfoFilename, filterMemFreeString));

  const long loc_totalMemory = (loc_totalMemoryFromFile == INVALID_VALUE) ? loc_lastTotalMemory : loc_totalMemoryFromFile;
  const long loc_freeMemory  = (loc_FreeMemoryFromFile == INVALID_VALUE)  ? loc_lastFreeMemory  : loc_FreeMemoryFromFile;

  //storing previous values
  loc_lastTotalMemory = loc_totalMemory;
  loc_lastFreeMemory  = loc_freeMemory;
  
  return (loc_totalMemory == 0) ? 0: static_cast<float>( loc_totalMemory - loc_freeMemory ) / loc_totalMemory;  // to avoid divide by 0 
}
// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  long loc_retVal = 0;
  long loc_Uptime;
  static long loc_lastUptime = 0;

  std::istringstream linestream(GetLineFromFile(kProcDirectory + kUptimeFilename));
  if(!(linestream >> loc_Uptime).fail()){
    loc_retVal = loc_Uptime;
  }
  else
  {
    loc_retVal = loc_lastUptime;
  }
  loc_lastUptime = loc_Uptime;
  return loc_retVal; }

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies(const CPUStates arg_CPUState) { 
  vector<long> loc_Jiffies = CpuUtilization();
  return (loc_Jiffies[arg_CPUState]); }

// DONE: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
  long loc_totalTime = 0;
  string loc_dumpvalues;
  std::istringstream linestream(GetLineFromFile(kProcDirectory + to_string(pid) + kStatFilename));
  long loc_uTime, loc_sTime, loc_cuTime, loc_csTime;
  loc_uTime  = 0;
  loc_sTime  = 0;
  loc_cuTime = 0;
  loc_csTime = 0;

  for(int i = 0;i < PID_UTIME_POSITION;i++)
  {
    if(!(linestream>>loc_dumpvalues)){
      return INVALID_JIFFIE_VALUE;
    }
  }
  if(linestream>>loc_uTime>>loc_sTime>>loc_cuTime>>loc_csTime){
    loc_totalTime = loc_uTime + loc_sTime + loc_cuTime + loc_csTime;
  }
  else
  {
    return INVALID_JIFFIE_VALUE;
  }
  
  return loc_totalTime/sysconf(_SC_CLK_TCK); 
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies( ) {   
  return (Jiffies(kUser_) + Jiffies(kNice_) + Jiffies(kSystem_) + Jiffies(kIRQ_) + Jiffies(kSoftIRQ_) + Jiffies(kSteal_));}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies( ) { 
  return (Jiffies(kIdle_) + Jiffies(kIOwait_)); }

// DONE: Read and return CPU utilization
vector<long> LinuxParser::CpuUtilization() { 
  string loc_cpu;
  long loc_jiffies;
  vector<long> loc_retVal;
  std::istringstream linestream(GetLineFromFile(kProcDirectory + kStatFilename));
  linestream >> loc_cpu;
  if(loc_cpu == filterCpu){
    for(int i = 0; i < CPUStates::CPUStates_MAX_;i++)
    { 
      if(!(linestream >> loc_jiffies).fail()){
        loc_retVal.push_back(loc_jiffies);
      }
      else{

      }
    }
  }
  return loc_retVal; }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses(){
  return (GetValueFromFile(kProcDirectory + kStatFilename, filterProcesses));
}
// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  return (GetValueFromFile(kProcDirectory + kStatFilename, filterRunningProcesses));
}
// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  return GetLineFromFile(kProcDirectory + to_string(pid) + kCmdlineFilename); 
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  //Using VmRSS instead of VmSize based on the review comments
  //"VmSize is the sum of all the virtual memory as you can see on the manpages also.
  //Whereas when you use VmRSS then it gives the exact physical memory being used as a part of Physical RAM.
  // So it is recommended to replace the string VmSize with VmRSS"
   const long loc_RamInKB = (GetValueFromFile(kProcDirectory + to_string(pid) + kStatusFilename, filterProcMem)); 
   return (loc_RamInKB == INVALID_VALUE) ? INVALID_RAM : to_string(loc_RamInKB / 1000);
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value = INVALID_USER;

  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.find(filterUID)!= std::string::npos)
      {
        std::istringstream linestream(line);

        if(!(linestream >> key >> value).fail()){
          break;
        }
        else
        {
          //do nothing
        }        
      }
    }
  }
  
  return value; }

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) { 
  string loc_retVal = INVALID_USER;
  const string loc_Uid= Uid(pid);
  string line, loc_userName, loc_x, loc_currUid;
  std::ifstream filestream(kPasswordPath);
  if(loc_Uid != INVALID_USER)
  {
    if(filestream.is_open()){
      while(std::getline(filestream, line)){
        std::replace(line.begin(), line.end(), ':', ' ');
        std::istringstream linestream(line);
        linestream >> loc_userName >> loc_x >> loc_currUid;
        if(loc_currUid == loc_Uid)
        {
          loc_retVal = loc_userName;
          break;
        }
      }
    }
  }
  else{
    //do nothing
  }
  
  return loc_retVal; }

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
  string line = GetLineFromFile(kProcDirectory + to_string(pid) + kStatFilename);
  long loc_PID_uptime = 0;
  string values;
  std::istringstream linestream(line);
  for(int i = 0; i <PID_UPTIME_POSITION; i++)
  {
    if(!(linestream >> values))
    {
      return INVALID_UPTIME_VALUE;      
    }
  }
  loc_PID_uptime = stol(values);

  return (loc_PID_uptime / sysconf(_SC_CLK_TCK)); }
