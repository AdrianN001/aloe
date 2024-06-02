#ifndef __FILE_SEARCH_WINDOW_H
#define __FILE_SEARCH_WINDOW_H

#include <ncurses.h>
#include "aloe/fs.h"


void start_file_search_window_popup(int start_x, int start_y, dir_t* directory, file_t* file_list, char* project_directory);
void render_result_to_popup_page(WINDOW* popup_page, file_search_result_t* result, size_t pointer_position, char* project_directory);



#endif