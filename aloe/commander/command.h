#ifndef __COMMAND_H_
#define __COMMAND_H_

#include <ncurses.h>
#include "aloe/fs.h"


typedef struct command {
    char* name;
    char* description;
    int shortcut;

    void (*callback)(WINDOW*, file_list_t*, dir_t*);
} command_t;



#endif