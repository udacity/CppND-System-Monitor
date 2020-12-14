#include <string>

#include "format.h"

using std::string;

// Output string to 2 place precision
string Format::Time(int time) {
  string Time = std::to_string(time);
  return string(2 - Time.length(), '0') + Time;
}

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) 
{
    
    int hour =0;
    int minute =0;
    int second =0;

    hour = seconds/3600;
    seconds %= 3600; //get the remaining number of seconds that can be converted to mins
    minute = seconds/60;
    seconds %= 60; // get the remaining number of seconds that will only fit as seconds
    second = seconds;
    
    
    return Time(hour)+":"+Time(minute)+":"+Time(second);
}