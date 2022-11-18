#include<iostream>
#include "linux_parser.h"
#include "format.h"
#include "system.h"
using namespace LinuxParser;
int main() {
    LinuxParser::MemoryUtilization();
    TotalProcesses();
    RunningProcesses();
    std::cout<<UpTime()<<std::endl;
    
    std::cout<<Format::ElapsedTime(1407245.84)<<std::endl;
    CpuUtilization();

    //test syste,
    System system;
    std::cout<<"cpu percentage: "<<system.Cpu().Utilization()<<std::endl;
    
    std::cout<<"cpu percentage: "<<system.Cpu().Utilization()<<std::endl;
    auto pids = LinuxParser::Pids();
    std::cout<<"number of pids: "<<pids.size()<<std::endl;
    std::cout<<"uid of process 1: "<<LinuxParser::Uid(1)<<std::endl;
    std::cout<<"user name: "<<LinuxParser::User(1)<<std::endl;
    std::cout<<"command "<<LinuxParser::Command(1)<<std::endl;
    std::cout<<"uptime "<<LinuxParser::UpTime(1095)<<std::endl;
    
    return 0;
}