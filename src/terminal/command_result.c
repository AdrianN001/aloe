#include "aloe/terminal.h"
#include <string.h>

terminal_command_result_t create_terminal_command_result(size_t size_of_buffer){
    return (terminal_command_result_t){
        .exit_code = -1,
        .exit_message = malloc(sizeof(char) * size_of_buffer)
    };
}

void set_exit_message_terminal_command_result(terminal_command_result_t* result, char* message, size_t len){
    strncpy(result->exit_message, message, len);
}


void free_terminal_command_result(terminal_command_result_t* res){
    free(res->exit_message);
}