#include <string>

using namespace std;
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
private:
    string pid;
    string user;
    string cmd;
    string cpu;
    string mem;
    string upTime;

public:
    Process(string pid){
        this->pid = pid;
        this->user = ProcessParser::getProcUser(pid);
        //TODOs:
        //complete for mem
        this->mem = ProcessParser::getVmSize(pid);
        //complete for cmd
        this->cmd = ProcessParser::getCmd(pid);
        //complete for upTime
        this->upTime = ProcessParser::getProcUpTime(pid);
        //complete for cpu
        this->cpu = ProcessParser::getCpuPercent(pid);
    }

    //complete getters and setters
    void setPid(int pid);
    string getPid()const;
    string getUser()const {return this->user;};
    string getCmd()const {return this->cmd;};
    int getCpu()const {return stoi(this->cpu);};
    int getMem()const {return stoi(this->mem);};
    string getUpTime()const {return this->upTime;};
    string getProcess();
};
void Process::setPid(int pid){
    this->pid = pid;
}
string Process::getPid()const {
    return this->pid;
}
string Process::getProcess(){
    if(!ProcessParser::isPidExisting(this->pid))
        return "";
    this->mem = ProcessParser::getVmSize(this->pid);
    this->upTime = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);

    //TODO: finish the string! this->user + "   "+ mem...cpu...upTime...;
    return (this->pid + "\t" + 
            this->user.substr(0, 5) + "\t" + 
            this->cpu.substr(0, 5) + "\t" + 
            this->mem.substr(0, 5) + "\t" + 
            Util::convertToTime (stoi(this->upTime)).substr(0, 8) + "\t" + 
            this->cmd.substr(0, 30) + "...");
}
