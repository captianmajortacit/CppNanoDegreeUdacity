#ifndef NCURSES_DISPLAY_H
  #include "ncurses_display.h"
#endif

#ifndef SYSTEM_H
  #include "system.h"
#endif

int main() {
  System system;
  NCursesDisplay::Display(system);
}