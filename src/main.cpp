#include <iostream>

#include "format.h"
#include "ncurses_display.h"
#include "system.h"

int main() {
  System system;
  //std::cout << "My OS is: " << system.OperatingSystem()<<std::endl;
  //std::cout<<"Memory Utilization: "<<system.MemoryUtilization()<<"\n";
  //std::cout<<"Running proccesses: "<<system.RunningProcesses()<<"\n";
  //std::cout<<"Total processes: "<<system.TotalProcesses()<<"\n";
  NCursesDisplay::Display(system);
}