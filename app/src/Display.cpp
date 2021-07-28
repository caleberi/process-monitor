#include <curses.h>
#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include "../utils/Helpers.h"
#include "../include/Display.h"
#include "SysInfo.h"

using std::string;
using std::to_string;

// 50 bars uniformly displayed from 0 - 100 %
// 2% is one bar(|)
std::string NCursesDisplay::ProgressBar(float percent)
{
  std::string result{"0%"};
  int size{50};
  float bars{percent * size};

  for (int i{0}; i < size; ++i)
  {
    result += i <= bars ? '|' : ' ';
  }

  string display{to_string(percent * 100).substr(0, 4)};
  if (percent < 0.1 || percent == 1.0)
    display = " " + to_string(percent * 100).substr(0, 3);
  return result + " " + display + "/100%";
}

void NCursesDisplay::DisplaySystem(SysInfo &sys, WINDOW *window)
{
  sys.setAttributes();

  mvwprintw(window, 2, 2, getCString(("OS: " + sys.getOSName())));
  mvwprintw(window, 3, 2, getCString(("Kernel version: " + sys.getKernelVersion())));
  mvwprintw(window, 4, 2, getCString("CPU: "));
  wattron(window, COLOR_PAIR(1));
  wprintw(window, getCString(Util::getProgressBar(sys.getCpuPercent())));
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, 5, 2, getCString(("Other cores:")));
  wattron(window, COLOR_PAIR(1));
  std::vector<std::string> val = sys.getCoresStats();
  for (int i = 0; i < val.size(); i++)
  {
    mvwprintw(window, (6 + i), 2, getCString(val[i]));
  }
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, 10, 2, getCString(("Memory: ")));
  wattron(window, COLOR_PAIR(1));
  wprintw(window, getCString(Util::getProgressBar(sys.getMemPercent())));
  wattroff(window, COLOR_PAIR(1));
  mvwprintw(window, 11, 2, getCString(("Total Processes:" + sys.getTotalProc())));
  mvwprintw(window, 12, 2, getCString(("Running Processes:" + sys.getRunningProc())));
  mvwprintw(window, 13, 2, getCString(("Up Time: " + Helpers::convertToTime(sys.getUpTime()))));
  wrefresh(window);
}

void NCursesDisplay::DisplayProcesses(Processor procs, WINDOW *win)
{
  procs.refreshList();
  wattron(win, COLOR_PAIR(2));
  mvwprintw(win, 1, 2, "PID:");
  mvwprintw(win, 1, 9, "User:");
  mvwprintw(win, 1, 16, "CPU[%]:");
  mvwprintw(win, 1, 26, "RAM[MB]:");
  mvwprintw(win, 1, 35, "Uptime:");
  mvwprintw(win, 1, 44, "CMD:");
  wattroff(win, COLOR_PAIR(2));
  vector<std::string> processes = procs.getList();
  for (int i = 0; i < 10; i++)
  {
    mvwprintw(win, 2 + i, 2, getCString(processes[i]));
  }
}

void NCursesDisplay::Display(SysInfo sys, Processor procs)
{
  initscr();    // start curses mode
  noecho();     // not printing input values
  cbreak();     // Terminating on classic ctrl + c
  startColor(); // Enabling color change of text
  int yMax, xMax;
  getmaxyx(stdscr, yMax, xMax); // getting size of window measured in lines and columns(column one char length)
  WINDOW *sys_win = newwin(17, xMax - 1, 0, 0);
  WINDOW *proc_win = newwin(15, xMax - 1, 18, 0);

  init_pair(1, COLOR_BLUE, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);

  while (true)
  {
    box(sys_win, 0, 0);
    box(proc_win, 0, 0);
    writeSysInfoToConsole(sys, sys_win);
    getProcessListToConsole(procs, proc_win);
    wrefresh(sys_win);
    wrefresh(proc_win);
    refresh();
    sleep(1);
  }
  endwin();
}
