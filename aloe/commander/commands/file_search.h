#ifndef __COMMAND_FILE_SEARCH_H_
#define __COMMAND_FILE_SEARCH_H_

#include <ncurses.h>
#include "aloe/fs.h"


void file_search_callback(WINDOW* base_window, file_list_t* file_list, dir_t* workspace_directory);



#endif