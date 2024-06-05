#ifndef __COMMAND_LIST_H_
#define __COMMAND_LIST_H_

#include "command.h"

typedef struct { 

    command_t* commands;
    size_t n_of_commands;

} command_list_t;



command_list_t init_commands();
command_list_t filter_commands(command_list_t* command_list, char* command_name);

void free_command_list(command_list_t* command_list);


#endif