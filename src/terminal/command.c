#include "aloe/terminal.h"
#include "aloe/buffer.h"
#include "aloe/terminal/commands.h"
#include <string.h>

#define ADD_NEW_TERMINAL_COMMAND(_name, _callback) ({\
                                                    terminal_command_t new_command = (terminal_command_t){.name = _name, .callback = _callback, };\
                                                    generic_buffer_append(&terminal_command_list, &new_command);\
                                                             })


terminal_command_list_t terminal_command_list;
static const int number_of_commands = 10;

void initilize_terminal_command_list(void){
    terminal_command_list = generic_buffer_init(sizeof(terminal_command_t), number_of_commands);
    load_terminal_commands(&terminal_command_list);
}


void load_terminal_commands(terminal_command_list_t* term_cmd_list){
    ADD_NEW_TERMINAL_COMMAND("nf", new_file_terminal_callback);
    ADD_NEW_TERMINAL_COMMAND("nd", new_dir_terminal_callback);
}

terminal_command_result_t try_to_execute_terminal_command( terminal_command_list_t* term_cmd_list, char* input, WINDOW* main_window, file_list_t* file_list, dir_t* workspace){
    char command_name[16] = {0};
    char args[64] = {0};
    terminal_command_result_t result;

    sscanf(input, "%[a-z] %[a-zA-Z0-9.-?~!+:_# ]", command_name, args);

    terminal_command_callback_t callback = NULL;
    
    for(int i = 0; i < (term_cmd_list->pointer / term_cmd_list->size_of_data); i++){
        terminal_command_t current_command = *(terminal_command_t*)(generic_buffer_at(term_cmd_list, i));

        if(!strcmp(command_name, current_command.name)){
            callback = current_command.callback;
            break;
        }
    }
    if(callback == NULL){
        return (terminal_command_result_t){.exit_code = -1};
    }

    callback(args, main_window, file_list, workspace, &result);
    return result;
}


void deinitalize_terminal_command_list(void){
    generic_buffer_free(&terminal_command_list);
}

#undef ADD_NEW_TERMINAL_COMMAND