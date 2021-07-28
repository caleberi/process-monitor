#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process
{
private:
    std::string pid;
    std::string user;
    std::string cmd;
    std::string cpu;
    std::string mem;
    std::string upTime;

public:
    Process(string pid)
    {
        this->pid = pid;
        this->user = ProcessParser::getProcUser(pid);
        this->mem = ProcessParser::getVmSize(pid);
        this->cmd = ProcessParser::getCmd(pid);
        this->upTime = ProcessParser::getProcUpTime(pid);
        this->cpu = ProcessParser::getCpuPercent(pid);
    }
    void setPid(int pid);
    string getPid() const;
    string getUser() const;
    string getCmd() const;
    int getCpu() const;
    int getMem() const;
    string getUpTime() const;
    string getProcess();
};

void Process::setPid(int pid)
{
    this->pid = pid;
}
string Process::getPid() const
{
    return this->pid;
}
string Process::getProcess()
{
    this->mem = ProcessParser::getVmSize(this->pid);
    this->up_time = ProcessParser::getProcUpTime(this->pid);
    this->cpu = ProcessParser::getCpuPercent(this->pid);

    return (this->pid + "   " + this->user + "   " + this->mem.substr(0, 5) + "     " + this->cpu.substr(0, 5) + "     " + this->upTime.substr(0, 5) + "    " + this->cmd.substr(0, 30) + "...");
}

#endif