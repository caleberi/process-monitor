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
    std::string searchParams = "VmData";
    std::ifstream stream;
    std::string path{Path::basePath() + pid + Path::statusPath()};
    float dataSize;
    stream.open(path);
    while (std::getline(stream, line))
    {
        if (line.compare(0, searchParams.size(), searchParams) == 0)
        {
            std::istringstream buffer(line);
            std::istream_iterator<std::string> beg(buffer), end;
            std::vector<std::string> tokens{beg, end};
            dataSize = (stof(tokens[1]) / (1024 * 1024));
            break;
        }
    }
    return std::to_string(dataSize) + "GB";
}
#endif // PROCESSPARSER_H