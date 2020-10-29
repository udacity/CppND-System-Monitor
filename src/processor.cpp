#include "processor.h"
#include <vector>
#include <string>

using std::string;
using std::vector;
// TODO: Return the aggregate CPU utilization
float Processor::Utilization() 
{ 
    vector<string> cpuTime = LinuxParser::CpuUtilization();
    int guest_nice = std::stoi(cpuTime.back());
    cpuTime.pop_back();
    int guest = std::stoi(cpuTime.back());
    cpuTime.pop_back();
    int steal = std::stoi(cpuTime.back());
    cpuTime.pop_back();
    int softirq = std::stoi(cpuTime.back());
    cpuTime.pop_back();
    int irq = std::stoi(cpuTime.back());
    cpuTime.pop_back();

    int iowait = std::stoi(cpuTime.back());
    cpuTime.pop_back();
    int idle = std::stoi(cpuTime.back());
    cpuTime.pop_back();
    int system = std::stoi(cpuTime.back());
    cpuTime.pop_back();
    int nice = std::stoi(cpuTime.back());
    cpuTime.pop_back();
    int user = std::stoi(cpuTime.back());
    cpuTime.pop_back();

    user = user - guest;
    idle = idle + iowait;
    int systemAllTime = system+irq+softirq;
    int virtAllTime = guest+guest_nice;
    int totalTime = user + nice + idle + systemAllTime+ virtAllTime;

    float cpuUtilization = (totalTime - idle)/totalTime;
    return cpuUtilization;
}
