#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
std::string Time(int time); // to format time in 2 digits
std::string ElapsedTime(long seconds);  // TODO: See src/format.cpp
};                                    // namespace Format

#endif