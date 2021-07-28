#ifndef PROCESSPARSER_H
#define PROCESSPARSER_H
#include <algorithm>
#include <iostream>
#include <math.h>
#include <thread>
#include <chrono>
#include <iterator>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cerrno>
#include <cstring>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include "../constants/Constants.h"
#include "../utils/Helpers.h"

class ProcessParser
{
public:
    static std::string getCmd(std::string pid);
    static std::vector<std::string> getPidList();
    static std::string getVmSize(std::string pid);
    static std::string getCpuPercent(std::string pid);
    static long int getSysUpTime();
    static std::string getProcUpTime(std::string pid);
    static std::string getProcUser(std::string pid);
    static std::vector<std::string> getSysCpuPercent(std::string coreNumber = "");
    static float getSysRamPercent();
    static int ProcessParser::getNumbersOfCores();
    static std::string getSysKernelVersion();
    static int getTotalThreads();
    static int getTotalNumberOfProcesses();
    static int getNumberOfRunningProcesses();
    static std::string getOsName();
    static std::string printCpuStats(std::vector<std::string> value1, std::vector<std::string> value2);
};

std::string ProcessParser::getVmSize(std::string pid)
{
    std::string line;
    std::string searchParam = "VmData";
    std::ifstream stream;
    std::string path{Path::basePath() + pid + Path::statusPath()};
    float dataSize;
    stream.open(path);
    while (std::getline(stream, line))
    {
        if (line.compare(0, searchParam.size(), searchParam) == 0)
        {
            std::istringstream buffer(line);
            std::istream_iterator<std::string> beg(buffer), end;
            std::vector<std::string> tokens{beg, end};
            dataSize = (stof(tokens[1]) / (1024 * 1024));
            break;
        }
    }
    return std::to_string(dataSize) + "GB";
};

std::string ProcessParser::getCpuPercent(std::string pid)
{
    std::string line;
    float result;
    std::string path{Path::basePath() + pid + Path::statPath()};
    std::ifstream stream = Helpers::getStream(path);
    std::getline(stream, line);
    std::istringstream buffer(line);
    std::istream_iterator<std::string> beg(buffer), end;
    std::vector<std::string> tokens{beg, end};
    float upTime = std::stof(tokens[14]);
    float sTime = std::stof(tokens[15]);
    float csTime = std::stof(tokens[16]);
    float startTime = std::stof(tokens[21]);
    float upTime = ProcessParser::getSysUpTime();
    float frequency = sysconf(_SC_CLK_TCK);
    float totalTime = upTime + sTime + csTime + upTime;
    float seconds = upTime - (startTime / frequency);
    result = 100.0 * ((totalTime / frequency) / seconds);
    return std::to_string(result);
};

std::string ProcessParser::getProcUpTime(std::string pid)
{
    std::string line;
    std::ifstream stream;
    std::string path{Path::basePath() + pid + Path::statPath()};
    std::ifstream stream = Helpers::getStream(path);
    std::getline(stream, line);
    std::istringstream buffer(line);
    std::istream_iterator<std::string> beg(buffer), end;
    std::vector<std::string> tokens{beg, end};
    return std::to_string(std::stof(tokens[13]) / sysconf(_SC_CLK_TCK));
}

long int ProcessParser::getSysUpTime()
{
    std::string line;
    std::string path{Path::basePath() + Path::upTimePath()};
    std::ifstream stream = Helpers::getStream(path);
    std::getline(stream, line);
    std::istringstream buffer(line);
    std::istream_iterator<std::string> beg(buffer), end;
    std::vector<std::string> tokens{beg, end};
    return std::stoi(tokens[0]);
};

std::string ProcessParser::getProcUser(std::string pid)
{
    std::string line;
    std::string result;
    std::string searchParam = "Uid:";
    std::string path{Path::basePath() + pid + Path::statusPath()};
    std::ifstream stream = Helpers::getStream(path);
    while (std::getline(stream, line))
    {
        if (line.compare(0, searchParam.length(), searchParam) == 0)
        {
            std::istringstream buffer(line);
            std::istream_iterator<std::string> beg(buffer), end;
            std::vector<std::string> tokens{beg, end};
            result = tokens[1];
            break;
        }
    }
    stream = Helpers::getStream("/etc/passwd");
    searchParam = ("x:" + result);
    while (std::getline(stream, line))
    {
        if (line.find(searchParam) != std::string::npos)
        {
            result = line.substr(0, line.find(":"));
            return result;
        }
    }
    return "";
};

std::vector<std::string> ProcessParser::getPidList()
{
    DIR *dir;
    std::vector<std::string> container;
    if (!(dir = opendir("/proc")))
        throw std::runtime_error(std::strerror(errno));
    while (dirent *dirp = readdir(dir))
    {
        if (dirp->d_type != DT_DIR)
            continue;
        if (std::all_of(dirp->d_name, dirp->d_name + std::strlen(dirp->d_name), [](char c)
                        { return !isdigit(c); }))
            container.push_back(dirp->d_name);
    }
    if (closedir(dir))
        throw std::runtime_error(std::strerror(errno));
    return container;
};

std::string ProcessParser::getCmd(std::string pid)
{
    std::string line;
    std::ifstream stream;
    stream = Helpers::getStream(Path::basePath() + pid + Path::statusPath());
    std::getline(stream, line);
    return line;
};

int ProcessParser::getNumbersOfCores()
{
    std::string line;
    std::string searchParam = "cpu cores";
    std::ifstream stream;
    std::string path{Path::basePath() + "cpuinfo"};
    float dataSize;
    stream.open(path);
    while (std::getline(stream, line))
    {
        if (line.compare(0, searchParam.size(), searchParam) == 0)
        {
            std::istringstream buffer(line);
            std::istream_iterator<std::string> beg(buffer), end;
            std::vector<std::string> tokens{beg, end};
            return stoi(tokens[3]);
        }
    }
    return 0;
};

std::vector<std::string> ProcessParser::getSysCpuPercent(std::string coreNumber)
{
    std::string line;
    std::string name = "cpu" + coreNumber;
    std::ifstream stream = Helpers::getStream((Path::basePath() + Path::statPath()));
    while (std::getline(stream, line))
    {
        if (line.compare(0, name.size(), name) == 0)
        {
            std::istringstream buf(line);
            std::istream_iterator<std::string> beg(buf), end;
            std::vector<std::string> values(beg, end);
            return values;
        }
    }
    return (std::vector<std::string>());
};

std::string ProcessParser::printCpuStats(std::vector<std::string> values1, std::vector<std::string> values2)
{
    /*
Because CPU stats can be calculated only if you take measures in two different time,
this function has two parameters: two vectors of relevant values.
We use a formula to calculate overall activity of processor.
*/
    float activeTime = getSysActiveCpuTime(values2) - getSysActiveCpuTime(values1);
    float idleTime = getSysIdleCpuTime(values2) - getSysIdleCpuTime(values1);
    float totalTime = activeTime + idleTime;
    float result = 100.0 * (activeTime / totalTime);
};

float getSysActiveCpuTime(std::vector<std::string> values)
{
    return (std::stof(values[CPUstates::S_USER]) +
            std::stof(values[CPUstates::S_NICE]) +
            std::stof(values[CPUstates::S_SYSTEM]) +
            std::stof(values[CPUstates::S_IRQ]) +
            std::stof(values[CPUstates::S_SOFTIRQ]) +
            std::stof(values[CPUstates::S_STEAL]) +
            std::stof(values[CPUstates::S_GUEST]) +
            std::stof(values[CPUstates::S_GUEST_NICE]));
}

float getSysIdleCpuTime(std::vector<std::string> values)
{
    return (std::stof(values[CPUstates::S_IDLE]) + stof(values[CPUstates::S_IOWAIT]));
}

float ProcessParser::getSysRamPercent()
{
    std::string line;
    std::vector<std::string> searchParams = {"MemTotal:", "MemFree:", "Buffers:"};
    std::string value;
    int result;
    float totalMemory = 0;
    float freeMemory = 0;
    float bufferMemory = 0;
    std::ifstream stream = Helpers::getStream((Path::basePath() + Path::memoInfoPath()));
    while (std::getline(stream, line))
    {
        if (totalMemory != 0 && freeMemory != 0)
        {
            break;
        }
        if (line.compare(0, searchParams[0].size(), searchParams[0]) == 0)
        {
            std::istringstream buffer(line);
            std::istream_iterator<std::string> beg(buffer), end;
            std::vector<std::string> tokens{beg, end};
            totalMemory = stof(tokens[1]);
        }

        if (line.compare(0, searchParams[1].size(), searchParams[1]) == 0)
        {
            std::istringstream buffer(line);
            std::istream_iterator<std::string> beg(buffer), end;
            std::vector<std::string> tokens{beg, end};
            freeMemory = stof(tokens[1]);
        }

        if (line.compare(0, searchParams[3].size(), searchParams[3]) == 0)
        {
            std::istringstream buffer(line);
            std::istream_iterator<std::string> beg(buffer), end;
            std::vector<std::string> tokens{beg, end};
            bufferMemory = stof(tokens[1]);
        }
    }
    return (100.0 * (1 - (freeMemory / (totalMemory - bufferMemory))));
};

std::string ProcessParser::getSysKernelVersion()
{
    std::string line;
    std::string searchParam = "Linux version ";
    std::ifstream stream = Helpers::getStream((Path::basePath() + Path::versionPath()));
    while (std::getline(stream, line))
    {
        if (line.compare(0, searchParam.size(), searchParam) == 0)
        {
            std::istringstream buffer(line);
            std::istream_iterator<std::string> beg(buffer), end;
            std::vector<std::string> tokens{beg, end};
            return tokens[2];
        }
    }
    return "";
};

std::string ProcessParser::getOsName()
{
    std::string line;
    std::string searchParam = "PRETTY_NAME=";
    std::ifstream stream = Helpers::getStream("/etc/os-release");
    while (std::getline(stream, line))
    {
        if (line.compare(0, searchParam.size(), searchParam) == 0)
        {
            std::size_t found = line.find("=");
            found++;
            std::string result = line.substr(found);
            result.erase(std::remove(result.begin(), result.end(), '"'), result.end());
            return result;
        }
    }
    return "";
}

int ProcessParser::getTotalThreads()
{
    std::string line;
    int result = 0;
    std::string searchParam = "Threads:";
    std::vector<std::string> pidList = getPidList();
    for (auto pid : pidList)
    {
        std::ifstream stream = Helpers::getStream((Path::basePath() + pid + Path::statusPath()));
        while (std::getline(stream, line))
        {
            if (line.compare(0, searchParam.size(), searchParam) == 0)
            {
                std::istringstream buffer(line);
                std::istream_iterator<std::string> beg(buffer), end;
                std::vector<std::string> tokens{beg, end};
                result += stoi(tokens[1]);
                break;
            }
        }
    }
    return result;
}

int ProcessParser::getTotalNumberOfProcesses()
{
    std::string line;
    int result = 0;
    std::string searchParam = "processes:";
    std::ifstream stream = Helpers::getStream((Path::basePath() + Path::statPath()));
    while (std::getline(stream, line))
    {
        if (line.compare(0, searchParam.size(), searchParam) == 0)
        {
            std::istringstream buffer(line);
            std::istream_iterator<std::string> beg(buffer), end;
            std::vector<std::string> tokens{beg, end};
            result += stoi(tokens[1]);
            break;
        }
    }
    return result;
};

int ProcessParser::getNumberOfRunningProcesses()
{
    std::string line;
    int result = 0;
    std::string searchParam = "procs_running:";
    std::ifstream stream = Helpers::getStream((Path::basePath() + Path::statPath()));
    while (std::getline(stream, line))
    {
        if (line.compare(0, searchParam.size(), searchParam) == 0)
        {
            std::istringstream buffer(line);
            std::istream_iterator<std::string> beg(buffer), end;
            std::vector<std::string> tokens{beg, end};
            result += stoi(tokens[1]);
            break;
        }
    }
    return result;
};

int test_get_pid_list()
{
    int counter = 0;
    for (int i = 0; i < ProcessParser::getPidList().size(); i++)
    {
        std::cout << ProcessParser::getPidList().at(i) << std::endl;
        counter++;
        if (counter > 10)
        {
            std::cout << std::endl;
            counter = 0;
        }
    }
}

#endif // PROCESSPARSER_H