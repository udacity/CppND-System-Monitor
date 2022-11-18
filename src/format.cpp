#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) { 
    string second, minutes, hours;
    second = std::to_string(seconds % 60);
    minutes = std::to_string((seconds % 3600) / 60);
    hours = std::to_string(seconds / 3600);

    //leading zero
    string hh = string(2 - hours.length(), '0') + hours;
    string mm = string(2 - minutes.length(), '0') + minutes;
    string ss = string(2 - second.length(), '0') + second;
    return hh + ":" + mm + ":" + ss; 
}