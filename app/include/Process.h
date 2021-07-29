#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "ProcessParser.h"

/**
 * @brief  Creates a process with the pid and generates the process uptime , the vmsize 
 *              cmdline arg to run such process , memory used by usage
 * 
 */
class Process
{
private:
    // the process id
    std::string pid;
    // the user name of the current process
    std::string user;
    // the command line of the current process
    std::string cmd;
    // the cpu usage of the current process
    std::string cpu;
    // the memory usage of the current process
    std::string mem;
    // the virtual memory usage of the current process
    std::string upTime;

public:
    Process(std::string pid)
    {
        this->pid = pid;
        this->user = ProcessParser::getProcUser(pid);
        this->mem = ProcessParser::getVmSize(pid);
        this->cmd = ProcessParser::getCmd(pid);
        this->upTime = ProcessParser::getProcUpTime(pid);
        this->cpu = ProcessParser::getCpuPercent(pid);
    }
    void setPid(int pid);
    std::string getPid() const;
    std::string getUser() const;
    std::string getCmd() const;
    int getCpu() const;
    int getMem() const;
    std::string getUpTime() const;
    std::string getProcess();
};

/**
 * @brief get the process id
 **/
void Process::setPid(int pid)
{
    this->pid = pid;
}
/**
 * @brief get the process id
 * @return the process id
 * */
std::string Process::getPid() const
{
    return this->pid;
}

/**
 * @brief get the user name of the current process
 * @return the description  of the current process
 * */
std::string Process::getProcess()
{
    this->mem = ProcessParser::getVmSize(this->pid);
    this->upTime = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);

    return (this->pid + "   " + this->user + "   " + this->mem.substr(0, 5) + "     " + this->cpu.substr(0, 5) + "     " + this->upTime.substr(0, 5) + "    " + this->cmd.substr(0, 30) + "...");
}

#endif