#pragma once

// #include <algorithm>
#include <iostream>
// #include <math.h>
// #include <thread>
// #include <chrono>
// #include <iterator>
#include <string>
#include <string.h>     //for strlen on cstring...
//#include <stdlib.h>
//#include <stdio.h>
#include <vector>
#include <fstream>
// #include <sstream>
// #include <stdexcept>
// #include <cerrno>
// #include <cstring>
#include <dirent.h>
// #include <time.h>
#include <unistd.h>
// #include <constants.h>

#include "constants.h"
#include "util.h"

using namespace std;

class ProcessParser {
    public:
        //System wide

        //System uptime
        static long int getSysUpTime();

        //List of all PIDs
        static vector<string> getPidList();

        //PID specific

        //Return sum of all mapped memories in GB
        static string getVmSize(string pid);

        //Process total CPU usage
        static string getCpuPercent(string pid);

        //Process uptime in seconds
        static string getProcUpTime(string pid);

        //Process user name
        static string getProcUser(string pid);

        // static string getCmd(string pid);
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

//========== System wide ============

//System uptime (in s)
long int ProcessParser::getSysUpTime()
{
    string uptimeToken = Util::getToken(Path::basePath() + Path::upTimePath(), 0, ' ');
    return stol(uptimeToken);
}

//List of all PIDs
vector<string> ProcessParser::getPidList()
{
    vector <string> PIDs;

    if (DIR *dir = opendir(Path::basePath().c_str()))
    {
        while (struct dirent *entry = readdir(dir))
        {
            if ((entry->d_type & DT_DIR) == DT_DIR && !((entry->d_type & DT_LNK) == DT_LNK))
            {
                bool isValid = true;
                for (uint64_t idx = 0; idx < strlen(entry->d_name); idx++) 
                    if ((entry->d_name[idx] < '0') || (entry->d_name[idx] > '9'))
                    {
                        isValid = false;
                        break;
                    }
                
                if (isValid)
                    PIDs.push_back(string(entry->d_name));
            }
        }

        closedir(dir);
    }
    else
        throw invalid_argument("Processes dir could not be listed");

    return PIDs;
}


//========== PID ============

//Return sum of all mapped memories in GB
//  See: https://stackoverflow.com/questions/17174645/vmsize-physical-memory-swap
string ProcessParser::getVmSize(string pid){
    //Many processes do no have a VmSize field, so return NA str instead
    try
    {
        string memToken = Util::getToken(Path::statusPath(pid), "VmSize:", 0, 1, '\t');
        return to_string(stof(memToken) / float(1024 * 1024));
    }
    catch(const std::exception& e)
    {
        return "NA ";
    }
}

//Process uptime in seconds
string ProcessParser::getProcUpTime(string pid)
{
    string uptimeToken = Util::getToken(Path::statPath(pid), 13, ' ');
    return to_string(stof(uptimeToken) / sysconf(_SC_CLK_TCK));
}

//Process total CPU usage
string ProcessParser::getCpuPercent(string pid)
{
    float utime = stof(ProcessParser::getProcUpTime(pid));
    float stime = stof(Util::getToken(Path::statPath(pid), 14, ' '));
    float cutime = stof(Util::getToken(Path::statPath(pid), 15, ' '));
    float cstime = stof(Util::getToken(Path::statPath(pid), 16, ' '));
    float starttime = stof(Util::getToken(Path::statPath(pid), 21, ' '));
    float uptime = ProcessParser::getSysUpTime();
    float freq = sysconf(_SC_CLK_TCK);
    float total_time = utime + stime + cutime + cstime;
    float seconds = uptime - (starttime/freq);
    return to_string(100.0*((total_time/freq)/seconds));    
}

//Process user name
string ProcessParser::getProcUser(string pid)
{
    string uidToken = Util::getToken(Path::statusPath(pid), "Uid:", 0, 1, '\t');
    return Util::getToken(Path::passwdPath(), uidToken, 2, 0, ':');
}