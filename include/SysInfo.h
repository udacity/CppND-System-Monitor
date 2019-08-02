#ifndef SYSINFO_H
#define SYSINFO_H

#include <string>
#include <vector>
#include "ProcessParser.h"
#include "util.h"

using std::vector;
using namespace std;

class SysInfo {
    public:
        SysInfo();
        void setAttributes();
        void setLastCpuMeasures();
        string getMemPercent() const;
        long getUpTime() const;
        string getThreads() const;
        string getTotalProc() const;
        string getRunningProc() const;
        string getKernelVersion() const;
        string getOsName() const; 
        string getCpuPercent() const;
        void getOtherCores(int _size);
        void setCpuCoresStats();
        vector<string> getCoresStats() const;

    private:
        vector<string> lastCpuStats;
        vector<string> currentCpuStats;
        vector<string> coresStats;
        vector<vector<string>> lastCpuCoresStats;
        vector<vector<string>> currentCpuCoresStats;
        string cpuPercent;
        float memPercent;
        string osName;
        string kernelVer;
        long upTime;
        int totalProc;
        int runningProc;
        int threads;
};

#endif/* SYSINFO_H */
