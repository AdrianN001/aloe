#ifndef __COMMAND_LIST_H_
#define __COMMAND_LIST_H_

#include "aloe/commander.h"

typedef struct { 

    command_t* commands;
    size_t n_of_commands;

} command_list_t;



#endif