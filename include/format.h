#ifndef FORMAT_H
#define FORMAT_H

#include <string>

namespace Format {
/**
 * @brief This converts a long time in seconds to a string time of format HH:MM:SS
 *
 * @param[in] times: Long measuring seconds
 *
 * @return String with format HH:MM:SS for the time
 */
std::string ElapsedTime(long times);
};// namespace Format

#endif
