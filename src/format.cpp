#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int HH = seconds / 3600;
  int MM = (seconds - 3600 * HH) / 60;
  int SS = seconds - 3600 * HH - 60 * MM;
  string out =
      std::to_string(HH) + ":" + std::to_string(MM) + ":" + std::to_string(SS);
  return out;
}