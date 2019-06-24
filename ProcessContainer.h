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
    std::vector<std::string> pidList = ProcessParser::getPidList();
    this->_list.clear();
    for(int i=0;i<pidList.size();i++){
        //When refreshing the list, only include proceses that are "alive"
        //    the read steam may fail with what():  Non - existing PID
        try
        {
            Process proc(pidList[i]);
            this->_list.push_back(proc);
        }
        catch(...)
        {
            ;
        }        
    }
}
std::string ProcessContainer::printList(){
    std::string result="";
    for(int i=0;i<this->_list.size();i++){
        result += this->_list[i].getProcess(0.1);
    }
    return result;
}
std::vector<std::vector<std::string> > ProcessContainer::getList(){
    std::vector<std::vector<std::string>> values;
    int lastIndex = 0;
    int entrySize = 0;

    //build vectors of 10 processes entries
    for(int i=0; i<ProcessContainer::_list.size(); i++){
        //select only processes with more than 0.1% of CPU usage
        std::string processStr = ProcessContainer::_list[i].getProcess(0.1);
        if (processStr.size() > 0)
        {
            //add a sub-list every 10
            if (lastIndex % 10 == 0)
            {
                std::vector<std::string> subList;
                values.push_back(subList);
            }

            //Add process to sub-list
            values.back().push_back(processStr);
            lastIndex++;
            entrySize = processStr.size();
        }
    }

    //complete the last entry with blank to eraze the bottom of the list
    for (int i=values.back().size(); i < 10; i++)
        values.back().push_back(Util::emptyString(entrySize));

    return values;
}
