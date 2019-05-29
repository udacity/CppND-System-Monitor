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
private:
    std::ifstream stream;
    static int getFromStatus(string pid, string field);       //(mine) done
    template <typename T>
    static vector<T> getSpacedList(std::istringstream &line); //(mine) done
    static long get_sys_active_cpu_time(vector<int> values);
    static long get_sys_idle_cpu_time(vector<int>values);

public:
    static string getCmd(string pid);                               // done
    static vector<int> getPidList();                             
    static std::string getVmSize(string pid);                       // done
    static std::string getCpuPercent(string pid);                   // done               
    static float getSysUpTime();                                    // done
    static std::string getProcUpTime(string pid);                   // done
    static string getProcUser(string pid);                          // done
    static vector<string> getSysCpuPercent(string coreNumber = ""); // started
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


std::vector<int> ProcessParser::getPidList(){
    DIR* dir;
    vector<int> container;
    if(!(dir = opendir("/proc")))
        throw std::runtime_error(std::strerror(errno));

    while (dirent* dirp = readdir(dir)) {
        if (dirp->d_type != DT_DIR)
            continue;
        // Is every character of the name a digit?
        //cout << dirp->d_name << endl;
        try {    
            if (to_string(stoi(dirp->d_name)) == dirp->d_name)
                container.push_back(stoi(dirp->d_name));
        }
        catch (std::invalid_argument){
            continue;
        }
        
    }
    //Validating process of directory closing
    if (closedir(dir))
        throw std::runtime_error(std::strerror(errno));
    return container;
} 

bool ProcessParser::isPidExisting(string pid){ 
    vector<int> vec = ProcessParser::getPidList();
    return (std::find(vec.begin(), vec.end(), stoi(pid)) != vec.end());
}


int ProcessParser::getFromStatus(string pid, string field){
    std::ifstream s;
    Util::getStream(Path::BASE + pid + Path::STATUS_F, s);
    std::string line, key, value;
    while(getline(s, line)){
        //cout << line;
        std::istringstream ss(line);
        getline(ss, key, ':'); 
        
        if (key==field){
            int value;
            ss >> value;
            s.close();
            return value;
        }
    }
    s.close();
    return -1;
}

template <typename T>
vector<T> ProcessParser::getSpacedList(std::istringstream &line){
    vector<T> v;
    T val;
    while (line >> val){
        v.push_back(val);
    }
    return v;
}


std::string ProcessParser::getVmSize(string pid){
    int size = getFromStatus(pid, "VmSize");
    return std::to_string(size);
}

std::string ProcessParser::getProcUser(string pid){
    int uid = getFromStatus(pid, "Uid");
    struct passwd *pws;
    pws = getpwuid(uid);
    return pws->pw_name;
}

long ProcessParser::get_sys_active_cpu_time(vector<int> values)
{
    return ((values[S_USER]) +
            (values[S_NICE]) +
            (values[S_SYSTEM]) +
            (values[S_IRQ]) +
            (values[S_SOFTIRQ]) +
            (values[S_STEAL]) +
            (values[S_GUEST]) +
            (values[S_GUEST_NICE]));
}

long ProcessParser::get_sys_idle_cpu_time(vector<int>values)
{
    return ((values[S_IDLE]) + (values[S_IOWAIT]));
}

vector<string>  ProcessParser::getSysCpuPercent(string coreNumber){
    std::ifstream s;
    Util::getStream(Path::BASE + Path::STAT_F, s);
    std::string line, key, value, outs("test");
    std::string name = "cpu" + coreNumber;
    while(getline(s, line)){
        cout << line << endl;
        std::istringstream ss(line);
        getline(ss, key, ' '); 
        if (key.substr(0,name.size())==name){
            vector<int> v = getSpacedList<int>(ss);
            s.close();
            cout << v[0] << endl;
            cout << v[1] << endl;
            vector<string> vec;
            long active = get_sys_active_cpu_time(v);
            long idle = get_sys_idle_cpu_time(v);
            float percent = active / double (active + idle);
            vec.push_back(to_string(percent));
            vec.push_back(to_string(active));
            vec.push_back(to_string(idle));
            return vec;
        }
    }
    s.close();
    vector<string> vec;
    vec.push_back(outs);
    return vec;

}

string ProcessParser::getCmd(string pid){
    std::ifstream s;
    Util::getStream(Path::BASE + pid + Path::CMD_F, s);
    std::string line;
    while(getline(s, line)){
        //cout << line;
        s.close();
        return line;
    }
    s.close();
    return std::string(" ");
}



std::string ProcessParser::getProcUpTime(string pid){
    std::ifstream s;
    Util::getStream(Path::BASE + pid + Path::STAT_F, s);
    std::string line, key, value;
    while(getline(s, line)){
        //cout << line;
        std::istringstream buf(line);
        std::istream_iterator<string> beg(buf), end;
        std::vector<string> vals(beg, end);
        s.close();
        return to_string(stof(vals[13])/sysconf(_SC_CLK_TCK));
        }
    s.close();
    return std::string(" ");
}

std::string ProcessParser::getCpuPercent(string pid){
    std::ifstream s;
    Util::getStream(Path::BASE + pid + Path::STAT_F, s);
    std::string line, key, value;
    while(getline(s, line)){
        //cout << line;
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
        return to_string(result * 100);
        }
    s.close();
    return std::string(" ");
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