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
    static string getCmd(string pid);
    static vector<string> getPidList();
    static std::string getVmSize(string pid);                       // done
    static std::string getCpuPercent(string pid);                   // started
    static long int getSysUpTime();
    static std::string getProcUpTime(string pid);
    static string getProcUser(string pid);                          // done
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


