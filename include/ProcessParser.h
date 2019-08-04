#ifndef PROCESS_PARSER_H
#define PROCESS_PARSER_H

#include <constants.h>
#include <dirent.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

class ProcessParser {
 public:
  static std::string getCmd(std::string pid);
  static std::vector<std::string> getPidList();
  static std::string getVmsSize(std::string pid);
  static std::string getCpuPercent(std::string pid);
  static long int getSysUpTime();
  static std::string getProcUpTime(std::string pid);
  static std::string getProcUser(std::string pid);
  static std::vector<std::string> getSysCpuPercent(std::string coreNumber = "");
  static float getSysRamPercent();
  static std::string getSysKernelVersion();
  static int getTotalThreads();
  static int getTotalNumberOfProcesses();
  static int getNumberOfCores();
  static int getNumberOfRunningProcesses();
  static std::string getOsName();
  static std::string printCpuStats(std::vector<std::string> values1,
                                   std::vector<std::string> values2);
};

#endif
