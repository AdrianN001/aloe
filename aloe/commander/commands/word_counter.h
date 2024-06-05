#ifndef __COMMAND_WORD_COUNTER_H_
#define __COMMAND_WORD_COUNTER_H_

#include <ncurses.h>
#include "aloe/fs.h"

void word_counter_command_callback(WINDOW* base_window, file_list_t* file_list, dir_t* workspace_directory);


#endif 
