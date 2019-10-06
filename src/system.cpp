#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "util.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
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
// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { return processes_; }

// TODO: Return the system's kernel identifier (string)
std::string System::Kernel() { return string(); }

// TODO: Return the system's memory utilization
float System::MemoryUtilization() { return 0.0; }

// TODO: Return the operating system name
std::string System::OperatingSystem() { return this->osName; }

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return this->upTime;  }

void System::setAttributes() {
  // getting parsed data
  this->memPercent = ProcessParser::getSysRamPercent();
  this->upTime = ProcessParser::getSysUpTime();
  this->totalProc = ProcessParser::getTotalNumberOfProcesses();
  this->runningProc = ProcessParser::getNumberOfRunningProcesses();
  this->threads = ProcessParser::getTotalThreads();
  this->currentCpuStats = ProcessParser::getSysCpuPercent();
  this->cpuPercent =
      ProcessParser::printCpuStats(this->lastCpuStats, this->currentCpuStats);
  this->lastCpuStats = this->currentCpuStats;
  this->setCpuCoresStats();
}

void System::setCpuCoresStats() {
  // Getting data from files (previous data is required)
  for (unsigned int i = 0; i < this->currentCpuCoresStats.size(); i++) {
    this->currentCpuCoresStats[i] =
        ProcessParser::getSysCpuPercent(std::to_string(i));
  }
  for (unsigned int i = 0; i < this->currentCpuCoresStats.size(); i++) {
    // after acquirement of data we are calculating every core percentage of
    // usage
    this->coresStats[i] = ProcessParser::printCpuStats(
        this->lastCpuCoresStats[i], this->currentCpuCoresStats[i]);
  }
  this->lastCpuCoresStats = this->currentCpuCoresStats;
}

string System::getKernelVersion() const { return this->kernelVer; }

string System::getCpuPercent() const { return this->cpuPercent; }

vector<string> System::getCoresStats() const {
  vector<string> result = vector<string>();
  for (unsigned int i = 0; i < this->coresStats.size(); i++) {
    string temp = ("cpu" + to_string(i) + ": ");
    float check = stof(this->coresStats[i]);
    if (!check || this->coresStats[i] == "nan") {
      return vector<string>();
    }
    temp += Util::getProgressBar(this->coresStats[i]);
    result.push_back(temp);
  }
  return result;
}

string System::getMemPercent() const 
{
  return std::to_string(this->memPercent);
}

string System::getTotalProc() const 
{ 
    return std::to_string(this->totalProc); 
}

string System::getRunningProc() const 
{
    return std::to_string(this->runningProc);
}

void System::getOtherCores(int _size) 
{
  // when number of cores is detected, vectors are modified to fit incoming data
  this->coresStats = vector<string>();
  this->coresStats.resize(_size);
  this->lastCpuCoresStats = vector<vector<string>>();
  this->lastCpuCoresStats.resize(_size);
  this->currentCpuCoresStats = vector<vector<string>>();
  this->currentCpuCoresStats.resize(_size);
  for (int i = 0; i < _size; i++) 
  {
    this->lastCpuCoresStats[i] =
        ProcessParser::getSysCpuPercent(std::to_string(i));
  }
}

void System::setLastCpuMeasures() {
  this->lastCpuStats = ProcessParser::getSysCpuPercent();
}