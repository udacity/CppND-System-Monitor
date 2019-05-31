#pragma once

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
#include "consts.h"
#include <pwd.h>

using namespace std;

class ProcessParser{
public:
    std::ifstream stream;
    static long get_sys_active_cpu_time(vector<int> values); //given
    static long get_sys_idle_cpu_time(vector<int>values);    //given

public:
    static string getCmd(string pid);                               // done
    static vector<int> getPidList();                                // done
    static std::string getVmSize(string pid);                       // done
    static float getCpuPercent(string pid);                   // done               
    static float getSysUpTime();                                    // done
    static float getProcUpTime(string pid);                   // done
    static string getProcUser(string pid);                          // done
    static vector<int> getSysCpuPercent(string coreNumber = ""); // done
    static float getSysRamPercent();                                // done
    static string getSysKernelVersion();                            // done
    static int getTotalThreads();                                   // done
    static int getTotalNumberOfProcesses();                         // done
    static int getNumberOfRunningProcesses();                       // done
    static string getOSName();                                      // done
    static std::string PrintCpuStats(std::vector<int> values1, std::vector<int>values2); // given
    static bool isPidExisting(string pid);                          // done
    static int get_number_of_cores();                               // done
};


//////////////// System Info /////////////////

int ProcessParser::get_number_of_cores(){
    // Get the number of host cpu cores
    string line;
    string name = "cpu cores";
    ifstream stream;
    Util::getStream((Path::BASE + "cpuinfo"), stream);
    while (std::getline(stream, line)) {
        if (line.compare(0, name.size(),name) == 0) {
            istringstream buf(line);
            istream_iterator<string> beg(buf), end;
            vector<string> values(beg, end);
            return stoi(values[3]);
        }
    }
    return 0;
}

std::string ProcessParser::getOSName(){
    ifstream s;
    Util::getStream("/etc/os-release", s);
    std::string name = Util::getStringFromStream(s, "PRETTY_NAME", '=');
    s.close();
    return name.substr(1, name.size()-2);
}

float ProcessParser::getSysRamPercent(){
    ifstream s;
    Util::getStream(Path::BASE + Path::MEM_F, s);
    float mtotal = Util::getItemFromStream<float>(s, "MemTotal");
    float mavail = Util::getItemFromStream<float>(s, "MemAvailable");
    //float buffs = Util::getItemFromStream<float>(s, "Buffers");
    s.close();
    return 100.0 * (1-mavail/mtotal);
}

std::string ProcessParser::getSysKernelVersion(){
    ifstream s;
    Util::getStream(Path::BASE + Path::VERSION_F, s);
    std::string line;
    while(getline(s, line)){
        istringstream ss(line);
        vector<std::string> vec = Util::getSpacedList<std::string>(ss);
        s.close();
        return vec[2];
    }

}

int ProcessParser::getTotalThreads(){
    vector<int> pidlist = getPidList();
    int total {0};
    for (int pid : pidlist){
        ifstream s;
        Util::getStream(Path::BASE + to_string(pid) + Path::STATUS_F, s);
        int threads = Util::getItemFromStream<int>(s, "Threads");
        s.close();
        total += threads;
    }  
    return total;
}

int ProcessParser::getTotalNumberOfProcesses(){
    ifstream s;
    Util::getStream(Path::BASE + Path::STAT_F, s);
    int procs = Util::getItemFromStream<int>(s, "processes", ' ');
    s.close();
    return procs;
}

int ProcessParser::getNumberOfRunningProcesses(){
    ifstream s;
    Util::getStream(Path::BASE + Path::STAT_F, s);
    int procs = Util::getItemFromStream<int>(s, "procs_running", ' ');
    s.close();
    return procs;
}

string ProcessParser::PrintCpuStats(vector<int> values1, vector<int> values2)
{
/*
Because CPU stats can be calculated only if you take measures in two different time,
this function has two paramaters: two vectors of relevant values.
We use a formula to calculate overall activity of processor.
*/
    double a1 = get_sys_active_cpu_time(values1);
    double a2 = get_sys_active_cpu_time(values2);
    double at = a2 -a1;
    double i1 = get_sys_idle_cpu_time(values1);
    double i2 = get_sys_idle_cpu_time(values2);
    double it = i2 -i1;
    double result = 100.0*(at / (at+it));
    return to_string(result);
}

long ProcessParser::get_sys_active_cpu_time(vector<int> values){
    return ((values[S_USER]) +
            (values[S_NICE]) +
            (values[S_SYSTEM]) +
            (values[S_IRQ]) +
            (values[S_SOFTIRQ]) +
            (values[S_STEAL]) +
            (values[S_GUEST]) +
            (values[S_GUEST_NICE]));
}

long ProcessParser::get_sys_idle_cpu_time(vector<int>values){
    return ((values[S_IDLE]) + (values[S_IOWAIT]));
}

vector<int>  ProcessParser::getSysCpuPercent(string coreNumber){
    std::ifstream s;
    Util::getStream(Path::BASE + Path::STAT_F, s);
    std::string line, key, value, outs("test");
    std::string name = "cpu" + coreNumber;
    while(getline(s, line)){
        std::istringstream ss(line);
        getline(ss, key, ' '); 
        if (key.substr(0,name.size())==name){
            vector<int> v = Util::getSpacedList<int>(ss);
            s.close();
            return v;
        }
    }
    s.close();
    vector<int> vec;
    vec.push_back(-1);
    return vec;
}

float ProcessParser::getSysUpTime(){
    std::ifstream s;
    Util::getStream(Path::BASE + Path::UPTIME_F, s);
    std::string line, key, value;
    float uptime;
    s >> uptime;
    s.close();
    return uptime;
}

//////////////// Process Info /////////////////

std::vector<int> ProcessParser::getPidList(){
    DIR* dir;
    vector<int> container;
    if(!(dir = opendir("/proc")))
        throw std::runtime_error(std::strerror(errno));

    while (dirent* dirp = readdir(dir)) {
        if (dirp->d_type != DT_DIR)
            continue;
        try {    
            if (to_string(stoi(dirp->d_name)) == dirp->d_name)
                container.push_back(stoi(dirp->d_name));
        }
        catch (std::invalid_argument){
            continue;
        }   
    }
    if (closedir(dir))
        throw std::runtime_error(std::strerror(errno));
    return container;
} 

bool ProcessParser::isPidExisting(string pid){ 
    vector<int> vec = ProcessParser::getPidList();
    return (std::find(vec.begin(), vec.end(), stoi(pid)) != vec.end());
}

std::string ProcessParser::getVmSize(string pid){
    ifstream s;
    Util::getStream(Path::BASE + pid + Path::STATUS_F, s);
    int size = Util::getItemFromStream<int>(s, "VmSize");
    s.close();
    return std::to_string(size/1024);
}

std::string ProcessParser::getProcUser(string pid){
    ifstream s;
    Util::getStream(Path::BASE + pid + Path::STATUS_F, s);
    int uid = Util::getItemFromStream<int>(s,"Uid");
    struct passwd *pws;
    pws = getpwuid(uid);
    s.close();
    return pws->pw_name;
}

string ProcessParser::getCmd(string pid){
    std::ifstream s;
    Util::getStream(Path::BASE + pid + Path::CMD_F, s);
    std::string line;
    while(getline(s, line)){
        s.close();
        return line;
    }
    s.close();
    return std::string("");
}

float ProcessParser::getProcUpTime(string pid){
    std::ifstream s;
    Util::getStream(Path::BASE + pid + Path::STAT_F, s);
    std::string line, key, value;
    while(getline(s, line)){
        std::istringstream buf(line);
        std::istream_iterator<string> beg(buf), end;
        std::vector<string> vals(beg, end);
        s.close();
        return stof(vals[13])/sysconf(_SC_CLK_TCK);
        }
    s.close();
    return -1;
}

float ProcessParser::getCpuPercent(string pid){
    std::ifstream s;
    Util::getStream(Path::BASE + pid + Path::STAT_F, s);
    std::string line, key, value;
    while(getline(s, line)){
        std::istringstream buf(line);
        std::istream_iterator<string> beg(buf), end;
        std::vector<string> vals(beg, end);

        float utime = stof(vals[13]);
        float stime = stof(vals[14]);
        float cutime = stof(vals[15]);
        float cstime = stof(vals[16]);
        float start = stof(vals[21]);
        float sysup = ProcessParser::getSysUpTime();
        float ticks = sysconf(_SC_CLK_TCK);
        float total = (utime + stime + cutime + cstime) / ticks;
        float result = total/(sysup - (start/ticks));
        
        s.close();
        return result * 100.0;
        }
    s.close();
    return -1;
}

