#ifndef NCURSES_DISPLAY_H
#define NCURSES_DISPLAY_H

#include <curses.h>

#include "process.h"
#include "system.h"

namespace NCursesDisplay
{
    void Display(System &system, Processor &processor);
    void DisplaySystem(System &system, WINDOW *window);
    void DisplayProcesses(std::vector<Process> &processes, WINDOW *window);
    std::string ProgressBar(float percent);
}; // namespace NCursesDisplay

#endif