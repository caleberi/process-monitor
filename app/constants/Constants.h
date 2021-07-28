#ifndef  CONSTANTS_H
#define CONSTANTS_H

#include <string>

/**
 * Defines the constants used to define process manager
 * statuses representation.
 */
enum  CPUstates
{
    S_USER = 0,
    S_NICE,
    S_SYSTEM,
    S_IDLE,
    S_IOWAIT,
    S_IRQ,
    S_SOFTIRQ,
    S_STEAL,
    S_GUEST,
    S_GUEST_NICE,
};

/**
 * Base utility function for accessing all available path on the  
 * linux system. eg /proc/cpuinfo, /proc/meminfo, /proc/stat, etc.
 * specify location of the file to be read and used as data source
 * to create the process manager.
 **/
class Path
{
public:
    /**
     *  @returns the base path of the file to be read which is the process
     *           file "/proc".
     * */
    static std::string basePath()
    {
        return "/proc";
    }

    /**
     * @returns the path of the file to be by the command line
     * */
    static std::string cmdPath()
    {
        return "/cmdline";
    }

    /**
     * @returns the path of the file to be by the system current status
     * */
    static std::string statusPath()
    {
        return "/status";
    }

    /**
     * @returns the path of the file to be by the system current stat
     * */
    static std::string statPath()
    {
        return "stat";
    }

    /**
     * @returns the path of the file to be by the system current long running time
     * */
    static std::string upTimePath()
    {
        return "uptime";
    }

    /**
     * @returns the path of the file to be by the system current cpu memory usage
     * */
    static std::string memoInfoPath()
    {
        return "memoinfo";
    }

    /**
     * @returns the path of the file to be by the system version.
     * */
    static std::string versionPath()
    {
        return "version";
    }
};

#endif /* CONSTANTS_H */