#ifndef __USER_INTERFACE_H_
#define __USER_INTERFACE_H_

#include <ncurses.h>
#include "aloe/fs.h"
#include "mode.h"


typedef struct{

    WINDOW* text_editor_window;
    WINDOW* workspace_window;
    WINDOW* terminal_window;
    WINDOW* time_window;
    WINDOW* file_data_window;

    aloe_mode_t current_mode;


} user_interface_t;

user_interface_t create_user_interface(WINDOW* base_window, file_list_t* file_list, dir_t* workspace);

void user_interface_update_all(user_interface_t* ui, file_list_t*, dir_t*);
void user_interface_free_all_windows(user_interface_t* ui);



#endif