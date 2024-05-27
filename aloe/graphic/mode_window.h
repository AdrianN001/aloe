#ifndef __MODE_WINDOW_H_
#define __MODE_WINDOW_H_

#include <ncurses.h>
#include "aloe/fs.h"


WINDOW* start_mode_window(WINDOW* base);

void update_mode_window(WINDOW* window, int current_mode);


#endif