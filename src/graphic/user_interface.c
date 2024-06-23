#include "aloe/graphic.h"
#include "aloe/assert.h"
#include <ncurses.h>


#define FREE_WINDOW(window)\
    {\
    werase(window);\
    wrefresh(window);\
    delwin(window);\
    }\

user_interface_t create_user_interface(WINDOW* base_window, file_list_t* file_list, dir_t* workspace){
    user_interface_t ui = {
        .file_data_window =     start_file_info_window(base_window, file_list->active_file), 
        .terminal_window  =     start_terminal_window(base_window),
        .text_editor_window =   start_file_editor_window(base_window),
        .workspace_window =     start_workspace_window(base_window, workspace),
        .time_window =          start_time_window(base_window)
    };

    update_file_editor_window(&ui, file_list, (int)NULL);

    return ui;
}


void user_interface_update_all(user_interface_t* ui, file_list_t* file_list, dir_t* workspace){
    update_file_editor_window(ui, file_list, (int)NULL);
    update_workspace_window(ui->workspace_window, workspace, file_list, (int)NULL);

    update_time_window(ui->time_window);
    update_file_info_window(ui->file_data_window, file_list->active_file);

}

 void user_interface_free_all_windows(user_interface_t* ui){
    FREE_WINDOW( ui->text_editor_window)
    FREE_WINDOW( ui->workspace_window)
    FREE_WINDOW( ui->terminal_window)
    FREE_WINDOW( ui->time_window)
    FREE_WINDOW( ui->file_data_window)
}

#undef FREE_WINDOW
