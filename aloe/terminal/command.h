#ifndef __TERMINAL_COMMAND_H_
#define __TERMINAL_COMMAND_H_

#include <ncurses.h>
#include "aloe/fs.h"
#include "aloe/buffer.h"
#include "command_result.h"

typedef void (*terminal_command_callback_t)(char* args, WINDOW*, file_list_t*, dir_t*, terminal_command_result_t* res);
typedef generic_buffer_t terminal_command_list_t;

typedef struct {
    char* name;

    terminal_command_callback_t callback;
} terminal_command_t;

extern terminal_command_list_t terminal_command_list; 

void initilize_terminal_command_list(void);
void load_terminal_commands(terminal_command_list_t* terminal_command_list);
terminal_command_result_t try_to_execute_terminal_command(terminal_command_list_t* term_cmd_list, char* input, WINDOW* main_window, file_list_t* file_list, dir_t* workspace);

void deinitalize_terminal_command_list(void);



#endif