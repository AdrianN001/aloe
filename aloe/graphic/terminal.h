#ifndef __TERMNINAL_H_
#define __TERMNINAL_H_


#include <ncurses.h>

WINDOW* start_terminal_window(WINDOW* base);

void update_terminal_window(WINDOW* window, int character);

#endif