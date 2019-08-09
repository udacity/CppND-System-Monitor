#include <string>

#include "format.h"

using std::string;
using std::to_string;
// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  long HH = seconds / 60 / 60;
  long MM = (seconds / 60) % 60;
  long SS = seconds % 60;
  string elapsedTime = to_string(HH) + ":" + to_string(MM) + ":" + to_string(SS);
  return elapsedTime;
}