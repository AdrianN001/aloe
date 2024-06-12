#ifndef __FILE_EDITOR_H_
#define __FILE_EDITOR_H_

#include <ncurses.h>
#include "aloe/fs.h"
#include "aloe/buffer.h"



/* Keyboard mapping */

#ifdef  KEY_ENTER
#undef  KEY_ENTER
#endif


#ifdef KEY_TAB
#undef KEY_TAB
#endif

#ifdef KEY_BACKSPACE
#undef KEY_BACKSPACE
#endif


#ifdef KEY_ARROW_UP
#undef KEY_ARROW_UP
#endif

#ifdef KEY_ARROW_DOWN
#undef KEY_ARROW_DOWN
#endif

#ifdef KEY_ARROW_LEFT
#undef KEY_ARROW_LEFT
#endif

#ifdef KEY_ARROW_RIGHT
#undef KEY_ARROW_RIGHT
#endif

#define KEY_ENTER           (int)0x0A

#define KEY_TAB             (int)0x09
#define KEY_BACKSPACE       (int)0x07
#define KEY_ARROW_UP        (int)0x03  
#define KEY_ARROW_DOWN      (int)0x02 
#define KEY_ARROW_LEFT      (int)0x04 
#define KEY_ARROW_RIGHT     (int)0x05 
#define KEY_ESCAPE          (int)27


#ifndef CTRL
#define CTRL(c) ((c) & 31)
#endif

#define WANT_TO_SAVE        1
#define DONT_WANT_TO_SAVE   0
#define CANCEL              -1


WINDOW* start_file_editor_window(WINDOW* base_window);
void render_blank_screen(WINDOW* window);


// Search for a new line downwards to move the view to
void update_file_editor_window(WINDOW *window, file_list_t *file_list, int character);


void render_file_list(file_list_t *file_list, WINDOW *window);

void show_saved_popup_window(int start_x, int start_y);
int show_close_popup_window(int start_x, int start_y);

buffer_t show_new_file_save_window(int start_x, int start_y);

#endif