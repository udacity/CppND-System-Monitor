#include "ncurses.h"
#include "ncurses_display.h"
#include "system.h"
#include "util.h"
#include <unistd.h>
#include <cstring>

char *getCString(std::string str) {
  char *cstr = new char[str.length() + 1];
  std::strcpy(cstr, str.c_str());
  return cstr;
}

void writeSysInfoToConsole(System sys, WINDOW *sys_win) {
  sys.setAttributes();

  mvwprintw(sys_win, 2, 2, getCString(("OS: " + sys.OperatingSystem())));
  mvwprintw(sys_win, 3, 2,
            getCString(("Kernel version: " + sys.getKernelVersion())));
  mvwprintw(sys_win, 4, 2, getCString("CPU: "));
  wattron(sys_win, COLOR_PAIR(1));
  wprintw(sys_win, getCString(Util::getProgressBar(sys.getCpuPercent())));
  wattroff(sys_win, COLOR_PAIR(1));
  mvwprintw(sys_win, 5, 2, getCString(("Other cores:")));
  wattron(sys_win, COLOR_PAIR(1));
  std::vector<std::string> val = sys.getCoresStats();
  for (unsigned int i = 0; i < val.size(); i++) {
    mvwprintw(sys_win, (6 + i), 2, getCString(val[i]));
  }
  wattroff(sys_win, COLOR_PAIR(1));
  mvwprintw(sys_win, 10, 2, getCString(("Memory: ")));
  wattron(sys_win, COLOR_PAIR(1));
  wprintw(sys_win, getCString(Util::getProgressBar(sys.getMemPercent())));
  wattroff(sys_win, COLOR_PAIR(1));
  mvwprintw(sys_win, 11, 2,
            getCString(("Total Processes:" + sys.getTotalProc())));
  mvwprintw(sys_win, 12, 2,
            getCString(("Running Processes:" + sys.getRunningProc())));
  mvwprintw(sys_win, 13, 2,
            getCString(("Up Time: " + Util::convertToTime(sys.UpTime()))));
}

void getProcessListToConsole(System Sys, WINDOW *win) {
  Sys.refreshList();
  wattron(win, COLOR_PAIR(2));
  mvwprintw(win, 1, 2, "PID:");
  mvwprintw(win, 1, 9, "User:");
  mvwprintw(win, 1, 16, "CPU[%]:");
  mvwprintw(win, 1, 26, "RAM[MB]:");
  mvwprintw(win, 1, 35, "Uptime:");
  mvwprintw(win, 1, 44, "CMD:");
  wattroff(win, COLOR_PAIR(2));
  vector<std::string> processes = Sys.getList();
  for (int i = 0; i < 10; i++) {
    mvwprintw(win, 2 + i, 2, getCString(processes[i]));
  }
}

void printMain(System sys) 
{
  initscr();      // start curses mode
  noecho();       // not printing input values
  cbreak();       // Terminating on classic ctrl + c
  start_color();  // Enabling color change of text
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax);  // getting size of window measured in lines and
                                 // columns(column one char length)
  WINDOW *sys_win = newwin(17, xMax - 1, 0, 0);
  WINDOW *proc_win = newwin(15, xMax - 1, 18, 0);

  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);

  while (true)
  {
    box(sys_win, 0, 0);
    box(proc_win, 0, 0);
    writeSysInfoToConsole(sys, sys_win);
    getProcessListToConsole(sys, proc_win);
    wrefresh(sys_win);
    wrefresh(proc_win);
    refresh();
    sleep(1);
  }
  endwin();
}

int main(int argc, char *argv[])
{
  // Object which containts relevant methods and attributes regarding system
  // details
  System sys;
  // string s = writeToConsole(sys);
  printMain(sys);
  return 0;
}
