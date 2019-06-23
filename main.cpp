#include <iostream>
// #include <thread>
// #include <chrono>
// #include <string>
// #include <vector>
// #include <ncurses.h>
// #include <time.h>
// #include <sstream>
// #include <iomanip>
// #include "util.h"
// #include "SysInfo.h"
// #include "ProcessContainer.h"

// using namespace std;


// char* getCString(std::string str){
//     char * cstr = new char [str.length()+1];
//     std::strcpy (cstr, str.c_str());
//     return cstr;
// }
// void writeSysInfoToConsole(SysInfo sys, WINDOW* sys_win){
//     sys.setAttributes();

//     mvwprintw(sys_win,2,2,getCString(( "OS: " + sys.getOSName())));
//     mvwprintw(sys_win,3,2,getCString(( "Kernel version: " + sys.getKernelVersion())));
//     mvwprintw(sys_win,4,2,getCString( "CPU: "));
//     wattron(sys_win,COLOR_PAIR(1));
//     wprintw(sys_win,getCString(Util::getProgressBar(sys.getCpuPercent())));
//     wattroff(sys_win,COLOR_PAIR(1));
//     mvwprintw(sys_win,5,2,getCString(( "Other cores:")));
//     wattron(sys_win,COLOR_PAIR(1));
//     std::vector<std::string> val = sys.getCoresStats();
//     for(int i=0;i<val.size();i++){
//      mvwprintw(sys_win,(6+i),2,getCString(val[i]));
//     }
//     wattroff(sys_win,COLOR_PAIR(1));
//     mvwprintw(sys_win,10,2,getCString(( "Memory: ")));
//     wattron(sys_win,COLOR_PAIR(1));
//     wprintw(sys_win,getCString(Util::getProgressBar(sys.getMemPercent())));
//     wattroff(sys_win,COLOR_PAIR(1));
//     mvwprintw(sys_win,11,2,getCString(( "Total Processes:" + sys.getTotalProc())));
//     mvwprintw(sys_win,12,2,getCString(( "Running Processes:" + sys.getRunningProc())));
//     mvwprintw(sys_win,13,2,getCString(( "Up Time: " + Util::convertToTime(sys.getUpTime()))));
//     wrefresh(sys_win);
// }

// void getProcessListToConsole(std::vector<string> processes,WINDOW* win){

//     wattron(win,COLOR_PAIR(2));
//     mvwprintw(win,1,2,"PID:");
//     mvwprintw(win,1,9,"User:");
//     mvwprintw(win,1,16,"CPU[%%]:");
//     mvwprintw(win,1,26,"RAM[MB]:");
//     mvwprintw(win,1,35,"Uptime:");
//     mvwprintw(win,1,44,"CMD:");
//     wattroff(win, COLOR_PAIR(2));
//     for(int i=0; i< processes.size();i++){
//         mvwprintw(win,2+i,2,getCString(processes[i]));
//    }
// }
// void printMain(SysInfo sys,ProcessContainer procs){
// 	initscr();			/* Start curses mode 		  */
//     noecho(); // not printing input values
//     cbreak(); // Terminating on classic ctrl + c
//     start_color(); // Enabling color change of text
//     int yMax,xMax;
//     getmaxyx(stdscr,yMax,xMax); // getting size of window measured in lines and columns(column one char length)
// 	WINDOW *sys_win = newwin(17,xMax-1,0,0);
// 	WINDOW *proc_win = newwin(15,xMax-1,18,0);


//     init_pair(1,COLOR_BLUE,COLOR_BLACK);
//     init_pair(2,COLOR_GREEN,COLOR_BLACK);
//     int counter = 0;
//     while(1){
//     box(sys_win,0,0);
//     box (proc_win,0,0);
//     procs.refreshList();
//     std::vector<std::vector<std::string>> processes = procs.getList();
//     writeSysInfoToConsole(sys,sys_win);
//     getProcessListToConsole(processes[counter],proc_win);
//     wrefresh(sys_win);
//     wrefresh(proc_win);
//     refresh();
//     sleep(1);
//     if(counter ==  (processes.size() -1)){
//         counter = 0;
//     }
//     else {
//         counter ++;
//     }
//     }
// 	endwin();
// }
// int main( int   argc, char *argv[] )
// {
//  //Object which contains list of current processes, Container for Process Class
//     ProcessContainer procs;
// // Object which containts relevant methods and attributes regarding system details
//     SysInfo sys;
//     //std::string s = writeToConsole(sys);
//     printMain(sys,procs);
//     return 0;
// }

#include "constants.h"
#include "util.h"
#include "ProcessParser.h"

using namespace std;


int main(int argc, char *argv[])
{
    bool displayCores = true;
    bool displayRAM = true;
    bool displayPID = false;

    if (displayRAM)
    {
        cout << "RAM usage = " << ProcessParser::getSysRamPercent() << "%" << endl;
    }

    if (displayCores)
    {
        cout << "System uptime = " << ProcessParser::getSysUpTime() << "s ";
        int numberCore = ProcessParser::getNumberOfCores();
        cout << "Number of core = " << numberCore << endl;

        //Collect CPU info
        vector<vector<string>> prevTotalCPUinfo;
        for (uint32_t idx = 0; idx < numberCore; idx++)
            prevTotalCPUinfo.push_back(ProcessParser::getSysCpuPercent(to_string(idx)));
        uint32_t count = 1;
        while(count-- != 0)
        {
            sleep(1);
            vector<vector<string>> currTotalCPUinfo;
            for (uint32_t idx = 0; idx < numberCore; idx++)
            {
                vector<string> currCPUinfo = ProcessParser::getSysCpuPercent(to_string(idx));
                cout << "CPU" << idx << " : " << Util::getProgressBar(ProcessParser::printCpuStats(prevTotalCPUinfo[idx], currCPUinfo)) << endl;
                prevTotalCPUinfo[idx] = currCPUinfo;
            }

            cout << endl;
        }
    }

    if (displayPID)
    {
        string userFilter = "damien";

        //list available PID
        vector<string> PIDs = ProcessParser::getPidList();
        for (string currPID : PIDs)
        {
            try 
            {
                //filter PID for specified user
                string pidUser = ProcessParser::getProcUser(currPID);
                if (pidUser != userFilter)
                    continue;

                string cmdLine = ProcessParser::getCmd(currPID);
                string vmSize = ProcessParser::getVmSize(currPID);
                string upTime = ProcessParser::getProcUpTime(currPID);
                string percentCPU = ProcessParser::getCpuPercent(currPID);

                cout << "PID=" << currPID << " : " << cmdLine << endl;
                cout << "   Memory used = " << vmSize << "GB";
                cout << ", Uptime = " << upTime << "s";
                cout << ", CPU used = " << percentCPU << "%";
                cout << ", User name = " << pidUser << endl;
            }
            catch (...)
            {
                cout << "PID=" << currPID << "error while reading" << endl;
                continue;
            } 

        }
    }

    return 0;
}


