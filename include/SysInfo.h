#ifndef SYSINFO_H
#define SYSINFO_H

#include <string>
#include <vector>
#include "ProcessParser.h"
using std::vector;

class SysInfo {
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

    public:
        SysInfo() {
            /*
            Getting initial info about system
            Initial data for individual cores is set
            System data is set
            */
            this->getOtherCores(ProcessParser::getNumberOfCores());
            this->setLastCpuMeasures();
            this->setAttributes();
            this->osName = ProcessParser::getOsName();
            this->kernelVer = ProcessParser::getSysKernelVersion();
        }
        void setAttributes();
        void setLastCpuMeasures();
        string getMemPercent() const;
        long getUpTime() const;
        string getThreads() const;
        string getTotalProc() const;
        string getRunningProc() const;
        string getKernelVersion() const;
        string getOSName() const;
        string getCpuPercent() const;
        void getOtherCores(int _size);
        void setCpuCoresStats();
        vector<string> getCoresStats() const;
};

#endif/* SYSINFO_H */
