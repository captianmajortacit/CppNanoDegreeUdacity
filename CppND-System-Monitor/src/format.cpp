#include <string>

#ifndef FORMAT_H
  #include "format.h"
#endif

using std::string;

#define HOURS(sec)   (sec / 3600)
#define MINUTES(sec) ((sec % 3600) /60)
#define SECONDS(sec) (sec % 60)
// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {

    return ( ((HOURS(seconds)  < 10) ? ("0"+ std::to_string(HOURS(seconds)  )) : std::to_string(HOURS(seconds)  )) + ":" + 
             ((MINUTES(seconds)< 10) ? ("0"+ std::to_string(MINUTES(seconds))) : std::to_string(MINUTES(seconds))) + ":" + 
             ((SECONDS(seconds)< 10) ? ("0"+ std::to_string(SECONDS(seconds))) : std::to_string(SECONDS(seconds)))
           ); }