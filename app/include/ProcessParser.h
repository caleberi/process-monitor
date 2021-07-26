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
#include <constants.h>
#include "../constants/Constants.h"
#include "../utils/Helpers.h"

class ProcessParser
{
public:
    static std::string getCmd(std::string pid);
    static std::vector<std::string> getCmds(std::string pid);
    static std::string getVmSize(std::string pid);
    static std::string getCpuPercent(std::string pid);
    static long int getSysUptime();
    static std::string getProcUptime(std::string pid);
    static std::string getProcUser(std::string pid);
    static std::vector<std::string> ProcessParser::getPidList();
    static std::vector<std::string> getSysCpuPercents(std::string coreNumber = "");
    static float getSysRamPercent();
    static std::string getSysKernelVersion();
    static int getTotalThreads();
    static int getTotalNumberOfProcesses();
    static int getNumberOfRunningProcesses();
    static std::string getOsName();
    static std::string getOsVersion();
    static std::string getOsArchitecture();
    static std::string getOsDistribution();
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
    float upTime = ProcessParser::getSysUptime();
    float frequency = sysconf(_SC_CLK_TCK);
    float totalTime = upTime + sTime + csTime + upTime;
    float seconds = upTime - (startTime / frequency);
    result = 100.0 * ((totalTime / frequency) / seconds);
    return std::to_string(result);
};

std::string ProcessParser::getProcUptime(std::string pid)
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

long int ProcessParser::getSysUptime()
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
        {
            container.push_back(dirp->d_name);
        }
    }
    if (closedir(dir))
        throw std::runtime_error(std::strerror(errno));
    return container;
}
#endif // PROCESSPARSER_H