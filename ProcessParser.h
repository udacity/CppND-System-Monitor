#pragma once

// #include <algorithm>
#include <iostream>
// #include <math.h>
// #include <thread>
// #include <chrono>
// #include <iterator>
#include <string>
// #include <stdlib.h>
// #include <stdio.h>
// #include <vector>
// #include <fstream>
// #include <sstream>
// #include <stdexcept>
// #include <cerrno>
// #include <cstring>
// #include <dirent.h>
// #include <time.h>
// #include <unistd.h>
// #include <constants.h>

#include "constants.h"
#include "util.h"

using namespace std;

class ProcessParser {
    public:
        // static string getCmd(string pid);
        // static vector<string> getPidList();

        //Return sum of all mapped memories
        static string getVmSize(string pid);
        
        // static string getCpuPercent(string pid);
        // static long int getSysUpTime();
        // static string getProcUpTime(string pid);
        // static string getProcUser(string pid);
        // static vector<string> getSysCpuPercent(string coreNumber = "");
        // static float getSysRamPercent();
        // static string getSysKernelVersion();
        // // static int getNumberOfCores();
        // static int getTotalThreads();
        // static int getTotalNumberOfProcesses();
        // static int getNumberOfRunningProcesses();
        // static string getOsName();
        // static string printCpuStats(vector<string> values1, vector<string>values2);
        // // static bool isPidExisting(string pid);
};

//Return sum of all mapped memories 
//  See: https://stackoverflow.com/questions/17174645/vmsize-physical-memory-swap
string ProcessParser::getVmSize(string pid){
    return Util::getToken(Path::statusPath(pid), "VmSize:", 1, '\t');
}
