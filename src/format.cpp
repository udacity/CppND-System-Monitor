#include <string>
#include <sstream>
#include <iomanip>

#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) { 
    std::ostringstream stream;

    int h, m, s = 0;

    h = seconds / 3600;
    seconds = seconds % 3600;

    m = seconds / 60;
    seconds = seconds % 60;

    s = seconds;

    stream << std::setw(2) << std::setfill('0') << h;
    stream << ':';
    stream << std::setw(2) << std::setfill('0') << m;
    stream << ':';
    stream << std::setw(2) << std::setfill('0') << s;

    return stream.str();
}