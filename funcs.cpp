#include <iostream>
#include <fstream>
#include "util.h"
#include "consts.h"
#include "ProcessParser.h"
#include "Process.h"

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
    
    int pid = 4234;
    //cout << "\n Enter PID: ";
    //cin >> pid;
    string test = ProcessParser::getVmSize(to_string(pid));
    string user = ProcessParser::getProcUser(to_string(pid));
    cout  << user << " - " << test << endl;      
    vector<string> cpu = ProcessParser::getSysCpuPercent("");
    cout <<cpu.size();
    cout  << cpu[0] << " - " << cpu[1] << " - " << cpu[2] << endl;
    cout <<  ProcessParser::getCmd(to_string(pid)) << endl;
    cout << ProcessParser::getProcUpTime(to_string(pid)) << endl;
    cout << "Proc CPU percent:" << ProcessParser::getCpuPercent(to_string(pid)) << endl;
    cout << "System uptime:" << ProcessParser::getSysUpTime() << endl;
    cout << ProcessParser::isPidExisting(to_string(300)) << endl;
    cout << ProcessParser::isPidExisting(to_string(pid)) << endl;

    Process p(to_string(pid));
    cout << p.getProcess() << endl;
    vector<int> pidlist = ProcessParser::getPidList();
    cout << pidlist.size() << " processes running" << endl;
    for (int i =60;i<120;i++){
        Process p(to_string(pidlist[i]));
        cout << p.getProcess() << endl;
    }
    return 0;
}