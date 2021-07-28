#include "../include/Display.h"
#include "SysInfo.h"

int main()
{
  SysInfo system;
  NCursesDisplay::Display(system);
}