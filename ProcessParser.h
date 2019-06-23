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
    private:
        //Sum all active raw CPU stats from getSysCpuPercent()
        static float getSysActiveCpuTime(vector<string> values);

        //Sum all Idle raw CPU stats from getSysCpuPercent()
        static float getSysIdleCpuTime(vector<string>values);

    public:
        //System wide

        //System uptime
        static long int getSysUpTime();

        //Number of Core
        static int getNumberOfCores();

        //List of all PIDs
        static vector<string> getPidList();

        //Get Core Raw information (see CPUStates)
        static vector<string> getSysCpuPercent(string coreNumber = "");

        //Get Core Usage (diff of 2 raw information vectors from getSysCpuPercent())
        static string printCpuStats(vector<string> values1, vector<string>values2);

        //PID specific

        //Return sum of all mapped memories in GB
        static string getVmSize(string pid);

        //Process total CPU usage
        static string getCpuPercent(string pid);

        //Process uptime in seconds
        static string getProcUpTime(string pid);

        //Process user name
        static string getProcUser(string pid);

        //Process commadline
        static string getCmd(string pid);

        
        // static float getSysRamPercent();
        // static string getSysKernelVersion();
        // static int getTotalThreads();
        // static int getTotalNumberOfProcesses();
        // static int getNumberOfRunningProcesses();
        // static string getOsName();
        // // static bool isPidExisting(string pid);
};

//========== System wide ============

//System uptime (in s)
long int ProcessParser::getSysUpTime()
{
    string uptimeToken = Util::getToken(Path::basePath() + Path::upTimePath(), 0, ' ');
    return stol(uptimeToken);
}

//Number of Core
int ProcessParser::getNumberOfCores()
{
    //Use "siblings" field instead of "cpu cores" by default  as it will reflect 
    //  the hyperthreading number of cores available on a machine
    string numCoreToken;
    try 
    {
        numCoreToken = Util::getToken(Path::cpuinfoPath(), "siblings\t", 0, 1, ':');
    }
    catch(...)
    {
        numCoreToken = Util::getToken(Path::cpuinfoPath(), "cpu cores\t", 0, 1, ':');
    }
    return stoi(numCoreToken);
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

//Get Core Raw information (see CPUStates)
vector<string> ProcessParser::getSysCpuPercent(string coreNumber)
{
    vector<string> rawVect;
    for (uint32_t idx = 0; idx <= (uint32_t)S_GUEST_NICE; idx++)
        rawVect.push_back(Util::getToken(Path::systemStatPath(), "cpu" + coreNumber, 0, idx, ' '));
    return rawVect;
}

//Get Core Usage (diff of 2 raw information vectors from getSysCpuPercent())
string ProcessParser::printCpuStats(vector<string> values1, vector<string> values2)
{
/*
Because CPU stats can be calculated only if you take measures in two different time,
this function has two parameters: two vectors of relevant values.
We use a formula to calculate overall activity of processor.
*/
    float activeTime = getSysActiveCpuTime(values2) - getSysActiveCpuTime(values1);
    float idleTime = getSysIdleCpuTime(values2) - getSysIdleCpuTime(values1);
    float totalTime = activeTime + idleTime;
    float result = 100.0*(activeTime / totalTime);
    return to_string(result);
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

//Process commadline
string ProcessParser::getCmd(string pid)
{
    string cmdLine;
    getline(Util::getStream(Path::cmdPath(pid)), cmdLine);
    return cmdLine;
}

//=========== private ============

//Sum all active raw CPU stats from getSysCpuPercent()
float ProcessParser::getSysActiveCpuTime(vector<string> values)
{
    return (stof(values[S_USER]) + stof(values[S_NICE]) + stof(values[S_SYSTEM]) + stof(values[S_IRQ]) +
            stof(values[S_SOFTIRQ]) + stof(values[S_STEAL]) + stof(values[S_GUEST]) + stof(values[S_GUEST_NICE]));
}

//Sum all Idle raw CPU stats from getSysCpuPercent()
float ProcessParser::getSysIdleCpuTime(vector<string>values)
{
    return (stof(values[S_IDLE]) + stof(values[S_IOWAIT]));
}
