#include "aloe/commander.h"
#include "aloe/buffer.h"
#include "aloe/commander/commands.h"
#include "aloe/assert.h"
#include <regex.h>
#include <stdlib.h>
#include <stdio.h>



#ifndef CTRL
#define CTRL(c) ((c) & 31)
#endif

#define ADD_COMMAND(_name, _description, _shortcut, _callback, command_list) (command_list.commands[command_list.n_of_commands++] = (command_t){.name=_name, .description=_description, .shortcut=_shortcut, .callback=_callback})
#define REGEX_MATCHES(_rgx_obj_p, _pattern_to_match) ( regexec(_rgx_obj_p, _pattern_to_match, 0, NULL, 0) == 0 )



command_list_t init_commands(){
    command_list_t command_list = {.commands = malloc(sizeof(command_t) * 32), .n_of_commands=0};


    //           Name                Description                    Shortcut     Callback
    ADD_COMMAND("File finder 15",   "Searches for a file",          ((int)'s'),  file_search_callback,                  command_list);
    ADD_COMMAND("Word counter",     "Displays the number of words", ((int)NULL),        word_counter_command_callback,         command_list);
    ADD_COMMAND("New file",         "Creates a new file",           ((int)'n'),  new_file_command_callback,             command_list);



    return command_list;
}

command_list_t filter_commands(command_list_t* command_list, char* command_name){
    command_list_t filtered_command_list;

    regex_t regex_obj;
    (void)regcomp(&regex_obj, command_name, 0);

    for (int i = 0; i < command_list->n_of_commands; i++){
        if ( REGEX_MATCHES(&regex_obj, command_list->commands[i].name) || REGEX_MATCHES(&regex_obj, command_list->commands[i].description) ){
            filtered_command_list.commands[filtered_command_list.n_of_commands++] = command_list->commands[i];
        }
    }

    return filtered_command_list ;

}

void free_command_list(command_list_t* command_list){
    free(command_list->commands);
}

#undef REGEX_MATCHES
#undef ADD_COMMAND