#pragma once

#include <string>
using namespace std;

enum CPUStates{
    S_TOTAL = 0,
	S_USER = 1,
	S_NICE,
	S_SYSTEM,
	S_IDLE,
	S_IOWAIT,
	S_IRQ,
	S_SOFTIRQ,
	S_STEAL,
	S_GUEST,
	S_GUEST_NICE
};

namespace Path
{
    const std::string BASE = "/proc/";
    const std::string STATUS_F = "/status";
    const std::string STAT_F = "stat";
}


