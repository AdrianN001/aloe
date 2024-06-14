#ifndef __TERMINAL_COMMAND_RESULT_H_
#define __TERMINAL_COMMAND_RESULT_H_

#include <stdlib.h>

typedef struct {

    int exit_code;
    char* exit_message;

}terminal_command_result_t;

terminal_command_result_t create_terminal_command_result(size_t size_of_buffer);

/* Will be stored in the heap */
void set_exit_message_terminal_command_result(terminal_command_result_t* result, char* message, size_t len);

void free_terminal_command_result(terminal_command_result_t* res);

#endif