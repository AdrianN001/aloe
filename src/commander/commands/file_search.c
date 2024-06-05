#include "aloe/commander/commands.h"
#include "aloe/graphic.h"
#include "aloe/fs.h"


void file_search_callback(WINDOW* base_window, file_list_t* file_list, dir_t* workspace_directory){
    start_file_search_window_popup(workspace_directory, file_list);
    wrefresh(base_window);
}
