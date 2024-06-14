#include "aloe/terminal/commands.h"
#include <string.h>


void new_dir_terminal_callback(char* args, WINDOW* main_window, file_list_t* file_list, dir_t* workspace,  terminal_command_result_t* res){
    char* dir_path = args;
    terminal_command_result_t result = create_terminal_command_result(32);


    make_new_directory(dir_path);


    result.exit_code = 0;
    char* message = "New directory was successfully created";
    size_t len = strlen(message);
    set_exit_message_terminal_command_result(&result,message, len);
    *res = result;
}