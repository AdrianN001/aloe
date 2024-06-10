#include "aloe/terminal/commands.h"


void new_dir_terminal_callback(char* args, WINDOW* main_window, file_list_t* file_list, dir_t* workspace){
    char* dir_path = args;

    make_new_directory(dir_path);
}