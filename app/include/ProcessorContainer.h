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
    std::vector<std::vector<std::string>> getList();

private:
    std::vector<Process> _list;
};

/**
 * @brief Clears and re-populate the process list to refresh the
 *           terminal
 */
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

/**
 * @brief  print the current  Process List to display running processes
 * 
 * @return std::string 
 */
std::string ProcessContainer::printList()
{
    std::string result = "";
    for (auto i : _list)
    {
        result += i.getProcess();
    }
    return result;
}

/**
 * @brief  retrieves the list of the current process
 * 
 * @return  std::vector<std::vector<std::string>> 
 */
 std::vector<std::vector<std::string>> ProcessContainer ::getList()
{
  std::vector<std::vector<std::string>> values;
  std::vector<std::string> stringified_list;
  for (int i = 0; i < this->_list.size(); ++i) {
    stringified_list.push_back(this->_list[i].getProcess());
  }
  int last_index = 0;
  for (int i = 0; i < stringified_list.size(); ++i) {
    if (i > 0 && i % 10 == 0) {
      std::vector<std::string> sub(&stringified_list[i-10],
                                   &stringified_list[i]);
      values.push_back(sub);
      last_index = i;
    }
    if (i == (this->_list.size() - 1) && (i - last_index) < 10) {
      std::vector<std::string> sub(&stringified_list[last_index],
                                   &stringified_list[i+1]);  // list.end() ?
      values.push_back(sub);
    }
  }
  return values;
}

#endif // PROCESSORCONTAINER_H