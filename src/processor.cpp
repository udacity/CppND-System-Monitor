#include "processor.h"


CPU_INFO Processor::GetLatestCpuInfo() {
    CPU_INFO cur_info;
    cur_info.non_idle_time = LinuxParser::ActiveJiffies();
    cur_info.idle_time =  LinuxParser::IdleJiffies();
    cur_info.total_time = LinuxParser::Jiffies();
    return cur_info;
}

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    float cpu_percentage;
    cur_cpu_info_ = std::make_unique<CPU_INFO>(GetLatestCpuInfo());
    #define WARP_SUBTRACT(a,b) (((a) > (b)) ? (a) - (b) : 0)
    if (prev_cpu_info_ != nullptr) {
        unsigned long long int totald = WARP_SUBTRACT(cur_cpu_info_->total_time, prev_cpu_info_->total_time);
        unsigned long long int idled = WARP_SUBTRACT(cur_cpu_info_->idle_time, prev_cpu_info_->idle_time);
        // std::cout<<"idle diff:"<<idled<<std::end;
        unsigned long long int total_period = WARP_SUBTRACT(totald, idled);
        if (totald == 0) {
            // std::cout<<"totald == 0"<<std::endl;
            cpu_percentage = 0;
        } else {
            cpu_percentage = static_cast<float>(total_period) / static_cast<float>(totald);
        }
    }

    prev_cpu_info_ = std::move(cur_cpu_info_);
    // std::cout<<"cpu percentage: "<<cpu_percentage<<std::endl;
    return cpu_percentage; 
}