#ifndef __WORKSPACE_H_
#define __WORKSPACE_H_

#include <ncurses.h>
#include "aloe/fs.h"


WINDOW* start_workspace_window(WINDOW* base, dir_t* directory);

void update_workspace_window(WINDOW* window,dir_t* directory, int character_pressed);


#endif