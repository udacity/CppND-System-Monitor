#include "format.h"
#include <iomanip>
#include <string>

std::string Format::ElapsedTime(long seconds)
{
  long total_seconds = seconds % 60;
  long temp_minutes = seconds / 60;
  long total_minutes = temp_minutes % 60;
  long total_hours = temp_minutes / 60;
  std::stringstream ss;
  ss.fill('0');
  ss << std::setw(2) << total_hours << ":" << std::setw(2) << total_minutes
     << ":" << std::setw(2) << total_seconds;
  return ss.str();
}
