#include <string>

#include "format.h"

using std::string;

long int mod(int x, int m) {
    if(m == 0) {
      return 0;
    }

    int r = x%m;
    return r<0 ? r+m : r;
}

string Format::Time(int time) {
  string time_str = std::to_string(time);
  if (time_str.length() < 2) {
    return "0" + time_str;
  } else {
    return time_str;
  }
}

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
string Format::ElapsedTime(long seconds) {
  int h = seconds/3600;
  int m = (mod(seconds, 3600)/60);
  int s = mod(seconds, 60);

  return Format::Time(h) + ":" + Format::Time(m) + ":" + Format::Time(s);
}
