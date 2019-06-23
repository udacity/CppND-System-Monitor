#pragma once

#include <string>
using namespace std;

enum CPUStates{
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

class Path{

public:
    static string basePath() {
        return "/proc/";
    }
    static string cmdPath(){
        return "/cmdline";
    }
    static string statusPath(){
        return "/status";
    }
    static string statPath(){
        return "stat";
    }
    static string upTimePath(){
        return "uptime";
    }
    static string memInfoPath(){
        return "meminfo";
    }
    static string versionPath(){
        return "version";
    }
    static string passwdPath(){
        return "/etc/passwd";
    }
    static string cpuinfoPath(){
        return basePath() + "cpuinfo";
    }
    static string systemStatPath(){
        return basePath() + statPath();
    }

    //Overload for specic PID files locations

    static string statusPath(string pid){
        return basePath() + pid + statusPath();
    }

    static string cmdPath(string pid){
        return basePath() + pid + cmdPath();
    }    

    //Field references see : /proc/[pid]/stat in 
    //                       http://man7.org/linux/man-pages/man5/proc.5.html
    static string statPath(string pid){
        return basePath() + pid + "/" +  statPath();
    }    
};
