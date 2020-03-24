#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long times) { 
    long hours = times / 60 / 60;
    long minutes = times / 60 % 60;
    long seconds = times % 60;
    // There's probably better way to do the 0 padding
    string hh = (hours < 10 ? "0" : "") + std::to_string(hours);
    string mm = (minutes < 10 ? "0" : "") + std::to_string(minutes);
    string ss = (seconds < 10 ? "0" : "") + std::to_string(seconds);
    return hh + ":" + mm + ":" + ss;
}