#include <iostream>

#include "format.h"
#include "ncurses_display.h"
#include "system.h"

int main() {
  // Various function tests
  // --------------------------------------------------------
  long S = 5 * 3600 + 25 * 60 + 23;
  // should return 5:25:23;
  std::cout << Format::ElapsedTime(S) << std::endl;

  // -------------------------------------------------------------------------------
  // System system;
  // NCursesDisplay::Display(system);
}