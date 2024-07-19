#include "aloe/terminal/commands.h"
#include <string.h>


void new_dir_terminal_callback(char* args, WINDOW* main_window, file_list_t* file_list, dir_t* workspace,  terminal_command_result_t* res){
    char* dir_path = args;
    terminal_command_result_t result = create_terminal_command_result(32);

    size_t len_of_pwd = strlen(workspace->dir_path);
    size_t len_of_new_dir = strlen(dir_path);
    char* new_path = malloc(sizeof(char) * (len_of_new_dir + len_of_pwd + 1 + 1));
    sprintf(new_path, "%s/%s", workspace->dir_path, dir_path);
    result.exit_code = make_new_directory(new_path);
    free(new_path);


    char* message = "New directory was successfully created";
    size_t len = strlen(message);
    set_exit_message_terminal_command_result(&result,message, len);
    *res = result;
}