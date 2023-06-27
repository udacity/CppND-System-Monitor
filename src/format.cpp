#include <string>
#include <sstream>
#include <iomanip>
#include "format.h"

std::string Format::ElapsedTime(long seconds)
{
  long hours = seconds / 3600;
  seconds = seconds - hours * 3600;
  long minutes = seconds / 60;
  seconds = seconds - (minutes * 60);
  char sec_string[3];
  char min_string[3];
  snprintf(sec_string, 3, "%02ld", seconds);
  snprintf(min_string, 3, "%02ld", minutes);
  return std::to_string(hours) + ":" + min_string + ":" + sec_string;
}
