#include "aloe/terminal/commands.h"
#include <string.h>

void new_file_terminal_callback(char* args, WINDOW* main_window, file_list_t* file_list, dir_t* workspace,  terminal_command_result_t* res){
    char* file_name = args;
    terminal_command_result_t result = create_terminal_command_result(32);


    file_t file_created = create_new_file(file_name);
    assert(file_created.fp != NULL);

    file_list_append_premade(file_list, file_created);

    result.exit_code = 0;
    char* message = "New file was successfully created";
    size_t len = strlen(message);
    set_exit_message_terminal_command_result(&result,message, len);
    *res = result;
}