#include <iostream>
#include "linux_parser.h"
#include "ncurses_display.h"
#include "system.h"

int main()
{
  System system;
  NCursesDisplay::Display(system);
}