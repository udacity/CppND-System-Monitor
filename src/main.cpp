#include "../include/ncurses_display.h"
#include "../include/system.h"

int main() {
  System system;
  NCursesDisplay::Display(system);
}