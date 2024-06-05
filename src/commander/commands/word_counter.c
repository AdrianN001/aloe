#include "aloe/commander/commands.h"
#include "aloe/graphic.h"
#include "aloe/fs.h"


void word_counter_command_callback(WINDOW* base_window, file_list_t* file_list, dir_t* workspace_directory){
    start_word_counter_popup(file_list->active_file);
    wrefresh(base_window);
}