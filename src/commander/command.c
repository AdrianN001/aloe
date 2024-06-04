#include "aloe/commander.h"
#include "aloe/buffer.h"


command_list_t command_list;

#define ADD_COMMAND(_name, _description, _callback) (command_list.commands[command_list.n_of_commands++] = (command_t){.name=_name, .description=_description, .callback=_callback})

void init_commands(){
    ADD_COMMAND("File finder 15", "Searches for a file", NULL);
    ADD_COMMAND("Word counter", "Displays the number of words", NULL);
}

#undef ADD_COMMAND

