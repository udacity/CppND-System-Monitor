#include <string>

#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
#include <string>

#include "format.h"

using std::string;
using std::to_string;
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
    int h = 00;
    int m = 00;
    int s = 00;
    string hour, min, sec;

    h = seconds / (60 * 60);
    m = seconds%(60 * 60) / 60;
    s = seconds%(60 * 60) % 60;	

    if (h < 10) {hour = "0" + to_string(h); }
    else {hour = to_string(h); }

    if (m < 10) {min = "0" + to_string(m); }
    else {min = to_string(m); }

    if (s < 10) {sec = "0" + to_string(s); }
    else {sec = to_string(s); }

    string time = hour + ": " +  min + ": " + sec;

    return time;
}