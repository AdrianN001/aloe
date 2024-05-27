#ifndef __FILE_EDITOR_H_
#define __FILE_EDITOR_H_

#include <ncurses.h>
#include "aloe/fs.h"


/* Keyboard mapping */
#define KEY_ENTER           0x0A
#define KEY_TAB             0x09
#define KEY_BACKSPACE       0x07

#define KEY_ARROW_UP        0x03  
#define KEY_ARROW_DOWN      0x02 
#define KEY_ARROW_LEFT      0x04 
#define KEY_ARROW_RIGHT     0x05 


#ifndef CTRL
#define CTRL(c) ((c) & 037)
#endif


WINDOW* start_file_editor_window(WINDOW* base_window);

// Search for a new line upward to move the view to
int search_negative_row_offset(file_t* file);

// Search for a new line downwards to move the view to
void update_file_editor_window(WINDOW* window, file_list_t* file_list, int character);

void show_save_popup_window(int start_x, int start_y);

#endif 