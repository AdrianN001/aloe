#ifndef __COMMAND_NEW_FILE_H_
#define __COMMAND_NEW_FILE_H_

#include <ncurses.h>
#include "aloe/fs.h"


void new_file_command_callback(WINDOW* base_window, file_list_t* file_list, dir_t* workspace_directory);



#endif