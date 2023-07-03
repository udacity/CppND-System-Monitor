#include <string>

#include "format.h"

using std::string;

// Format time into HH:MM:SS
string Format::ElapsedTime(long seconds) {
  string hrs;
  string min;
  string sec;

  // Lambda: if value is < 10, prepend 0
  auto prepend =
  [&] (long time) {
      if (time < 10)  // Must always have 2 digits
        return "0" + std::to_string(time);
      return std::to_string(time);
    };

  // Calculate time chunks and convert to string
  hrs = prepend(seconds / 3600) + ":";
  min = prepend((seconds % 3600) / 60) + ":";
  sec = prepend(((seconds % 3600) % 60) % 60);

  return hrs + min + sec;
}