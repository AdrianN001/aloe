#include "aloe/graphic.h"
#include "aloe/assert.h"
#include <ncurses.h>


#define FREE_WINDOW(window)\
    {\
    werase(window);\
    wrefresh(window);\
    delwin(window);\
    }\


void user_interface_update_all(user_interface_t* ui, file_list_t* file_list, dir_t* workspace){
    update_file_editor_window(ui->text_editor_window, file_list, (int)NULL);
    update_workspace_window(ui->workspace_window, workspace, file_list, (int)NULL);

    update_time_window(ui->time_window);
    update_file_info_window(ui->file_data_window, file_list->active_file);

}

inline void user_interface_free_all_windows(user_interface_t* ui){
    FREE_WINDOW( ui->text_editor_window)
    FREE_WINDOW( ui->workspace_window)
    FREE_WINDOW( ui->terminal_window)
    FREE_WINDOW( ui->time_window)
    FREE_WINDOW( ui->file_data_window)
}

#undef FREE_WINDOW
