#ifndef __TERMINAL_GRAPHIC_H_
#define __TERMINAL_GRAPHIC_H_

#include <ncurses.h>

#include "user_interface.h"
#include "aloe/fs.h"
#include "aloe/terminal.h"
#include "aloe/buffer.h"

WINDOW* start_terminal_window(WINDOW* base);

void update_terminal_window(WINDOW* terminal_window, user_interface_t* ui,  int character, buffer_t* buffer, WINDOW* base_window, file_list_t* file_list, dir_t* workspace );

#endif