#include "format.h"

#include <string>

using std::string;

// DONE: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  string elapsedTime;

  elapsedTime = std::to_string(seconds / (60 * 60)) + ":" +
                std::to_string((seconds % (60 * 60)) / 60) + ":" +
                std::to_string(seconds % 60);

  return elapsedTime;
}