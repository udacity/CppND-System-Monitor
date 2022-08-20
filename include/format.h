#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
    std::string FullTimeFormat(long time);
    std::string ElapsedTime(long times);  // See src/format.cpp
};                                    // namespace Format

#endif