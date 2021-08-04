#ifndef HELPERS_H
#define HELPERS_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

class Helpers
{
public:
    static std::string convertToTime(long int input_second);
    static std::string getProgressBar(std::string percentage);
    static std::ifstream getStream(std::string path);
};

/**
 * @brief Converts the given time in seconds to a string
 * @param input_second The time in seconds
 * @return The time in a string format
 * */
std::string Helpers::convertToTime(long int input_second)
{
    long minutes = input_second / 60;
    long hours = minutes / 60;
    long seconds = int(input_second % 60);
    minutes = int(minutes % 60);
    std::stringstream time;
    time << hours << ":" << std::setfill('0') << std::setw(2) << minutes << ":" << std::setfill('0') << std::setw(2) << seconds;
    return time.str();
};

/**
 * @brief Returns a progress bar
 * @param percentage The percentage of the progress bar
 * @return The progress bar string 
 * */
std::string Helpers::getProgressBar(std::string percentage)
{
    // constructing string for given percentage
    // 50 bar is uniformly streched 0 - 100%
    // meaning every bar is 2% wide
    std::stringstream ret("0%");
    int _size = 50;
    int boundaries = static_cast<int>((std::stof(percentage) / 100) * _size);
    for (int i = 0; i < _size; i++)
    {
        if (i <= boundaries)
            ret << "|";
        else
            ret << " ";
    }
    ret << percentage.substr(0, 5) << "/100%";
    return ret.str();
};

/**
 * @brief Returns a file stream
 * @param path The path to the file
 * @return The file stream
 * */
std::ifstream Helpers::getStream(std::string path)
{
    // opening file stream
    std::ifstream stream(path);
    if (!stream.is_open()){
         std::cout << "Error **"<<std::endl;
        throw std::runtime_error("Could not open PID file [ " + path + " does not exist ]");
    }
    return stream;
}

#endif //f HELPERS_H

