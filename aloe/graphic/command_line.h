#ifndef __COMMANDER_GRAPHIC_H_
#define __COMMANDER_GRAPHIC_H_

#include "aloe/commander.h"
#include <ncurses.h>

void start_command_line(command_list_t*, WINDOW* base_window, file_list_t* file_list, dir_t* directory);
void render_command_results_to_popup(WINDOW* popup, command_list_t*, int cursor);


#endif