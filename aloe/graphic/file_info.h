#ifndef __FILE_INFO_WINDOW_H_
#define __FILE_INFO_WINDOW_H_

#include <ncurses.h>
#include "aloe/fs.h"


WINDOW* start_file_info_window(WINDOW* base, file_t* file);

void update_file_info_window(WINDOW* window, file_t* new_file);


#endif