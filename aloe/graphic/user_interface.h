#ifndef __USER_INTERFACE_H_
#define __USER_INTERFACE_H_

#include <ncurses.h>
#include "aloe/fs.h"


typedef struct{

    WINDOW* text_editor_window;
    WINDOW* workspace_window;
    WINDOW* terminal_window;
    WINDOW* time_window;
    WINDOW* file_data_window;


} user_interface_t;

void user_interface_update_all(user_interface_t* ui, file_list_t*, dir_t*);
inline void user_interface_free_all_windows(user_interface_t* ui);



#endif