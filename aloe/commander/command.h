#ifndef __COMMAND_H_
#define __COMMAND_H_

#include <ncurses.h>
#include "aloe/fs.h"


typedef struct command {
    char* name;
    char* description;

    void (*callback)(WINDOW*, file_list_t*, dir_t*);
} command_t;


void init_commands();


#endif