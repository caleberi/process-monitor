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

#endif // PROCESSPARSER_H