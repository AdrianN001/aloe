#ifndef __NEW_FILE_TERMINAL_COMMAND_H_
#define __NEW_FILE_TERMINAL_COMMAND_H_

#include <ncurses.h>
#include "aloe/fs.h"

void new_file_terminal_callback(char* args, WINDOW* main_window, file_list_t* file_list, dir_t* workspace);


#endif