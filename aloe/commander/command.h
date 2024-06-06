#ifndef __COMMAND_H_
#define __COMMAND_H_

#include <ncurses.h>
#include "aloe/fs.h"


typedef void (*command_callback_t)(WINDOW*, file_list_t*, dir_t*);

typedef struct command {
    char* name;
    char* description;
    int shortcut;

    command_callback_t callback;

} command_t;



#endif