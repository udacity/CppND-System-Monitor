#include "ncurses_display.h"
#include "system.h"
#include <iostream>
#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "linux_parser.h"


int main() {
  
  System system;
  NCursesDisplay::Display(system);
  
}