#ifndef __NEW_DIR_TERMINAL_COMMAND_H_
#define __NEW_DIR_TERMINAL_COMMAND_H_

#include <ncurses.h>
#include "../command_result.h"
#include "aloe/fs.h"
#include "aloe/assert.h"

void new_dir_terminal_callback(char* args, WINDOW* main_window, file_list_t* file_list, dir_t* workspace, terminal_command_result_t* res);

#endif