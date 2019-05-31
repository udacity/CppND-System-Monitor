#pragma once

#include "Process.h"
#include <vector>
class ProcessContainer{

private:
    std::vector<Process>_list;
public:
    ProcessContainer(){
        this->refreshList();
    }
    void refreshList();
    std::string printList();
    std::vector<std::vector<std::string>> getList();
};

void ProcessContainer::refreshList(){
    std::vector<int> pidList = ProcessParser::getPidList();
    this->_list.clear();
    for(int i=0;i<pidList.size();i++){
        try{
            Process proc(to_string(pidList[i]));
            string cmd = proc.getCmd();
            if (cmd.empty())
                continue;
            else
                this->_list.push_back(proc);
            }
        catch (std::runtime_error){
            continue;
        }
    }
}
std::string ProcessContainer::printList(){
    std::string result="";
    for(int i=0;i<this->_list.size();i++){
        result += this->_list[i].getProcess();
    }
    return result;
}
std::vector<std::vector<std::string> > ProcessContainer::getList(){
    std::vector<std::vector<std::string>> values;
    std::vector<std::string> stringifiedList;
    for(int i=0; i<ProcessContainer::_list.size(); i++){
        stringifiedList.push_back(ProcessContainer::_list[i].getProcess());
    }
    int lastIndex = 0;
    for (int i=0; i<stringifiedList.size();i++){
        if(i %25 == 0 && i > 0){
          std::vector<std::string>  sub(&stringifiedList[i-25], &stringifiedList[i]);
          values.push_back(sub);
          lastIndex = i;
        }
        if(i == (ProcessContainer::_list.size() - 1) && (i-lastIndex)<25){
            std::vector<std::string> sub(&stringifiedList[lastIndex],&stringifiedList[i+1]);
            values.push_back(sub);
        }
   }
    return values;
}
