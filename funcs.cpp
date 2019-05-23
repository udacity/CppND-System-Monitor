#include <iostream>
#include <fstream>
#include "util.h"
#include "consts.h"
#include "ProcessParser.h"

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
    
    int pid = 6546;
    //cout << "\n Enter PID: ";
    //cin >> pid;
    string test = ProcessParser::getVmSize(to_string(pid));
    string user = ProcessParser::getProcUser(to_string(pid));
    cout  << user << " - " << test << endl;      
    vector<string> cpu = ProcessParser::getSysCpuPercent("");
    cout <<cpu.size();
    cout  << cpu[0] << " - " << cpu[1] << " - " << cpu[2] << endl;   
    return 0;
}