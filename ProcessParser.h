#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include "constants.h"


using namespace std;

class ProcessParser{
    private:
        std::ifstream stream;
    public:
        static string getCmd(string pid);
        static vector<string> getPidList();
        static std::string getVmSize(string pid);
        static std::string getCpuPercent(string pid);
        static long int getSysUpTime();
        static std::string getProcUpTime(string pid);
        static string getProcUser(string pid);
        static vector<string> getSysCpuPercent(string coreNumber = "");
        static float getSysRamPercent();
        static string getSysKernelVersion();
        static int getTotalThreads();
        static int getTotalNumberOfProcesses();
        static int getNumberOfRunningProcesses();
        static string getOSName();
        static std::string PrintCpuStats(std::vector<std::string> values1, std::vector<std::string>values2);
        static bool isPidExisting(string pid);
};

// TODO: Define all of the above functions below:

// implement getVMSize according to Lesson10.
std::string ProcessParser::getVmSize(string pid) {
    std::string line;
    std::string name = "VmData";
    std::ifstream stream;
    
    Util::getStream((Path::basePath() + pid + Path::statusPath()), stream); //modified the syntax according to the different implementation of getStream in the actual "util.h" file

    while(std::getline(stream, line)) {
        if(line.compare(0, name.size(), name) == 0) {
            std::istringstream buf(line);
            std::istream_iterator<string> beg(buf), end;
            std::vector<string> values(beg, end);
            return to_string(stof(values[1])/1024.f);
        };
    };
    return "";
};