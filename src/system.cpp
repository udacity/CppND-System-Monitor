#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <iterator>
#include <algorithm>
#include <sstream>
#include "process.h"
#include "processor.h"
#include "system.h"
#include "util.h"
#include "constants.h"

using namespace std;
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
  this->getOtherCores(System::getNumberOfCores());
  this->setLastCpuMeasures();
  this->setAttributes();
  this->osName = this->getOsName();
  this->kernelVer = System::getSysKernelVersion();
  this->refreshList();
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
  this->memPercent = System::getSysRamPercent();
  this->upTime = Process::getSysUpTime();
  this->totalProc = Process::getTotalNumberOfProcesses();
  this->runningProc = Process::getNumberOfRunningProcesses();
  this->threads = Process::getTotalThreads();
  this->currentCpuStats = System::getSysCpuPercent();
  this->cpuPercent =
      Processor::printCpuStats(this->lastCpuStats, this->currentCpuStats);
  this->lastCpuStats = this->currentCpuStats;
  this->setCpuCoresStats();
}

void System::setCpuCoresStats() {
  // Getting data from files (previous data is required)
  for (unsigned int i = 0; i < this->currentCpuCoresStats.size(); i++) {
    this->currentCpuCoresStats[i] =
        System::getSysCpuPercent(std::to_string(i));
  }
  for (unsigned int i = 0; i < this->currentCpuCoresStats.size(); i++) {
    // after acquirement of data we are calculating every core percentage of
    // usage
    this->coresStats[i] = Processor::printCpuStats(
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
        System::getSysCpuPercent(std::to_string(i));
  }
}

void System::setLastCpuMeasures() {
  this->lastCpuStats = System::getSysCpuPercent();
}

void System::refreshList() {
  vector<string> pids = Process::getPidList();
  this->_list.clear();
  for (auto pid : pids) {
    Process proc(pid);
    this->_list.push_back(proc);
  }
}

string System::printList() {
  std::string result = "";
  for (auto i : _list) {
    result += i.getProcess();
  }
  return result;
}

vector<string> System::getList() {
  vector<string> values;
  for (unsigned int i = (this->_list.size() - 10); i < this->_list.size();
       i++) {
    values.push_back(this->_list[i].getProcess());
  }
  return values;
}

string System::getOsName() {
  string line;
  string name = "PRETTY_NAME=";

  ifstream stream = Util::getStream(("/etc/os-release"));

  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      std::size_t found = line.find("=");
      found++;
      std::string result = line.substr(found);
      result.erase(std::remove(result.begin(), result.end(), '"'),
                   result.end());
      return result;
    }
  }
  return "";
}

string System::getSysKernelVersion() {
  string line;
  string name = "Linux version ";
  ifstream stream = Util::getStream((Path::basePath() + Path::versionPath()));
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      istringstream buf(line);
      istream_iterator<string> beg(buf), end;
      vector<string> values(beg, end);
      return values[2];
    }
  }
  return "";
}

float System::getSysRamPercent() {
  string line;
  string name1 = "MemAvailable:";
  string name2 = "MemFree:";
  string name3 = "Buffers:";

  string value;
  // int result;
  ifstream stream = Util::getStream((Path::basePath() + Path::memInfoPath()));
  float total_mem = 0;
  float free_mem = 0;
  float buffers = 0;
  while (std::getline(stream, line)) {
    if (total_mem != 0 && free_mem != 0) break;
    if (line.compare(0, name1.size(), name1) == 0) {
      istringstream buf(line);
      istream_iterator<string> beg(buf), end;
      vector<string> values(beg, end);
      total_mem = stof(values[1]);
    }
    if (line.compare(0, name2.size(), name2) == 0) {
      istringstream buf(line);
      istream_iterator<string> beg(buf), end;
      vector<string> values(beg, end);
      free_mem = stof(values[1]);
    }
    if (line.compare(0, name3.size(), name3) == 0) {
      istringstream buf(line);
      istream_iterator<string> beg(buf), end;
      vector<string> values(beg, end);
      buffers = stof(values[1]);
    }
  }
  // calculating usage:
  return float(100.0 * (1 - (free_mem / (total_mem - buffers))));
}

vector<string> System::getSysCpuPercent(string coreNumber) 
{
  // It is possible to use this method for selection of data for overall cpu or
  // every core. when nothing is passed "cpu" line is read when, for example "0"
  // is passed  -> "cpu0" -> data for first core is read
  string line;
  string name = "cpu" + coreNumber;
  ifstream stream = Util::getStream((Path::basePath() + Path::statPath()));
  while (std::getline(stream, line)) 
  {
    if (line.compare(0, name.size(), name) == 0) 
    {
      istringstream buf(line);
      istream_iterator<string> beg(buf), end;
      vector<string> values(beg, end);
      // set of cpu data active and idle times;
      return values;
    }
  }
  return (vector<string>());
}

int System::getNumberOfCores() {
  // Get the number of host cpu cores
  string line;
  string name = "cpu cores";
  ifstream stream = Util::getStream((Path::basePath() + "cpuinfo"));
  while (std::getline(stream, line)) {
    if (line.compare(0, name.size(), name) == 0) {
      istringstream buf(line);
      istream_iterator<string> beg(buf), end;
      vector<string> values(beg, end);
      return stoi(values[3]);
    }
  }
  return 0;
}



