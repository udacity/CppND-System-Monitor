// This is just to test each completed function and verify output after changes
// not real unit-tests...

#include <iostream>
#include <fstream>
#include "util.h"
#include "consts.h"
#include "ProcessParser.h"
#include "Process.h"
#include "ProcessContainer.h"
#include "SysInfo.h"

using namespace std;

int main(){
    // string t = Util::convertToTime(64000);
    // cout << "test convertToTime: "<< t << "\n";
    
    // cout << "test progressbar: " << Util::getProgressBar(" 23%") << "\n";
    // cout << "test progressbar: " << Util::getProgressBar("100%") << "\n";
    
    // cout << "test getstream exists:"<< "\n";
    // string filename = "main.cpp";
    // ifstream s1;
    // Util::getStream(filename, s1);
    // string line;
    // getline(s1,line);
    // cout<< line << endl;
    // s1.close();
    // cout << "test getstream non-existing file:"<< "\n";
    // Util::getStream("bullshit", s1);
    

    //cout << "\n Enter PID: ";
    //cin >> pid;
    vector<int> pl = ProcessParser::getPidList();
    int pid = pl[pl.size()-1];

    string test = ProcessParser::getVmSize(to_string(pid));
    string user = ProcessParser::getProcUser(to_string(pid));
    cout  << user << " - " << test << endl;      
    vector<int> cpu = ProcessParser::getSysCpuPercent("");
    cout <<cpu.size();
    cout  << cpu[0] << " - " << cpu[1] << " - " << cpu[2] << endl;
    cout <<  ProcessParser::getCmd(to_string(pid)) << endl;
    cout << ProcessParser::getProcUpTime(to_string(pid)) << endl;
    cout << "Proc CPU percent:" << ProcessParser::getCpuPercent(to_string(pid)) << endl;
    
    cout << ProcessParser::isPidExisting(to_string(300)) << endl;
    cout << ProcessParser::isPidExisting(to_string(pid)) << endl;

    Process p(to_string(pid));
    cout << p.getProcess() << endl;
    vector<int> pidlist = ProcessParser::getPidList();
//    cout << pidlist.size() << " processes running" << endl;

    ProcessContainer PC;
    //cout << PC.printList()<< endl;
    
    cout << "\nSystem Info (not process related)" << endl;
    cout << "---------------------------------" << endl;
    cout << "System uptime: " << ProcessParser::getSysUpTime() << endl;
    cout << "num cores: " << ProcessParser::get_number_of_cores() << endl;
    cout << "os name: " << ProcessParser::getOSName() << endl;
    cout << "kernel version: " << ProcessParser::getSysKernelVersion() << endl;
    cout << "total threads: " << ProcessParser::getTotalThreads() << endl;
    cout << "total procs: " << ProcessParser::getTotalNumberOfProcesses() << endl;
    cout << "running procs: " << ProcessParser::getNumberOfRunningProcesses() << endl;
    cout << "mem percent: " << ProcessParser::getSysRamPercent() << endl;
    int corenum = 1;
    cout << "cpu" << corenum << " --> ";
    for (auto info : ProcessParser::getSysCpuPercent(to_string(corenum))){
        cout << info << " - ";
    }
    cout << endl;
    auto v1 = ProcessParser::getSysCpuPercent(to_string(corenum));
    cout << ProcessParser::get_sys_active_cpu_time(v1) << " - " << ProcessParser::get_sys_idle_cpu_time(v1) << endl;
    
    int slept = sleep(1);
    auto v2 = ProcessParser::getSysCpuPercent(to_string(corenum));
    cout << ProcessParser::get_sys_active_cpu_time(v2) << " - " << ProcessParser::get_sys_idle_cpu_time(v2) << endl;
    cout << "cpu1 usage: " << ProcessParser::PrintCpuStats(v1, v2) << endl;
    double a1 = ProcessParser::get_sys_active_cpu_time(v1);
    double a2 = ProcessParser::get_sys_active_cpu_time(v2);
    cout << a1 << " - " << a2 << endl;
    cout << a2 - a1 << endl;
    float testf = ProcessParser::get_sys_active_cpu_time(v2)-ProcessParser::get_sys_active_cpu_time(v1);
    cout << testf << endl;


    return 0;
}