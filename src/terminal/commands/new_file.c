#include "aloe/terminal/commands.h"
#include <string.h>

void new_file_terminal_callback(char* args, WINDOW* main_window, file_list_t* file_list, dir_t* workspace,  terminal_command_result_t* res){
    char* file_name = args;
    terminal_command_result_t result = create_terminal_command_result(32);


     size_t len_of_pwd = strlen(workspace->dir_path);
    size_t len_of_new_file = strlen(file_name);
    char* new_path = malloc(sizeof(char) * (len_of_new_file + len_of_pwd + 1 + 1));
    sprintf(new_path, "%s/%s", workspace->dir_path, file_name);

    file_t file_created = create_new_file(new_path);
    free(new_path);

    assert(file_created.fp != NULL);

    file_list_append_premade(file_list, file_created);

    result.exit_code = 0;
    char* message = "New file was successfully created";
    size_t len = strlen(message);
    set_exit_message_terminal_command_result(&result,message, len);
    *res = result;
}