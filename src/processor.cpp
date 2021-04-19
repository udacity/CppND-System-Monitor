#include "processor.h"
#include <iostream>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<std::string> cpu_values = LinuxParser::CpuUtilization();
     
    float total_time, idle_time = 0;
    for(std::string s : cpu_values){
        total_time += std::stof(s);
    }
    idle_time += std::stof(cpu_values[3]) + std::stof(cpu_values[4]);
    /*
    PU_Util.push_back(user);
    CPU_Util.push_back(nice);
    CPU_Util.push_back(sys);
    CPU_Util.push_back(idle);
    CPU_Util.push_back(iowait);
    CPU_Util.push_back(irq);
    CPU_Util.push_back(softirq);
    CPU_Util.push_back(steal);
    CPU_Util.push_back(guest);
    CPU_Util.push_back(guest_nice);
    */
   //float total_time, idle_time;
   //total_time = std::stof(cpu_values[0]) + std::stof(cpu_values[1])
    
    // Come back here and store values so that I can track this over time
    return (total_time - idle_time)/total_time; 
}