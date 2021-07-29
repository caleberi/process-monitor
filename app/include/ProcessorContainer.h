#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "Process.h"
#include "ProcessParser.h"
#include <string>
#include <vector>

/**
 * @brief  Wrap the Process class to provide \n a simple interface for 
 *          creating and managing a Process/Processes.
 * */
class ProcessContainer
{
public:
    ProcessContainer()
    {
        this->refreshList();
    }
    void refreshList();
    std::string printList();
    std::vector<std::string> getList();

private:
    std::vector<Process> _list;
};

void ProcessContainer ::refreshList()
{
    std::vector<std::string> pids = ProcessParser::getPidList();
    this->_list.clear();
    for (auto pid : pids)
    {
        Process proc(pid);
        this->_list.push_back(proc);
    }
}

std::string ProcessContainer ::printList()
{
    std::string result = "";
    for (auto i : _list)
    {
        result += i.getProcess();
    }
    return result;
}

std::vector<std::string> ProcessContainer ::getList()
{
    std::vector<std::string> values;
    for (int i = (this->_list.size() - 10); i < this->_list.size(); i++)
    {
        values.push_back(this->_list[i].getProcess());
    }
    return values;
}

#endif // PROCESSORCONTAINER_H