#include "aloe/terminal/commands.h"

void new_file_terminal_callback(char* args, WINDOW* main_window, file_list_t* file_list, dir_t* workspace){
    char* file_name = args;

    file_t file_created = create_new_file(file_name);
    assert(file_created.fp != NULL);

    file_list_append_premade(file_list, file_created);
}