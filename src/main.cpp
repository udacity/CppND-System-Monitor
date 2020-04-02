#include <iostream>
#include "linux_parser.h"
#include "ncurses_display.h"
#include "system.h"

int main()
{
  Processor p{LinuxParser::CpusUtilization().front()};
  System system(p);
  NCursesDisplay::Display(system);
}