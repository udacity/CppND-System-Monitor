#include <string>
#include <sstream>
#include <cmath>
#include <iostream>

#include "format.h"

using std::string;
using std::ostringstream;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function


string LeadingZero(long val){
    return val < 10 ? "0" : "";
}

string Format::ElapsedTime(long seconds) {
    ostringstream output;
    int hrs, mins, secs;
    hrs = round(seconds / 3600);
    mins = (seconds - (hrs * 3600)) / 60;
    secs = seconds - (hrs * 3600) - (mins * 60);
    output << LeadingZero(hrs) << hrs << ":" << LeadingZero(mins) << mins << ":"
            << LeadingZero(secs) << secs;
    return output.str(); 
}