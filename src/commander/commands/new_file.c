#include "aloe/commander/commands.h"
#include "aloe/graphic.h"
#include "aloe/fs.h"
#include "aloe/buffer.h"
#include "aloe/assert.h"




void new_file_command_callback(WINDOW* base_window, file_list_t* file_list, dir_t* workspace_directory){
    buffer_t file_name_buffer = show_new_file_save_window(LINES/2,  COLS/2);
    if (file_name_buffer.pointer == -1){
        buffer_free(&file_name_buffer);
        wrefresh(base_window);
        return;
    }
    wrefresh(base_window);

    char* file_name = file_name_buffer.data;

    file_t file_created = create_new_file(file_name);
    assert(file_created.fp != NULL);

    file_list_append_premade(file_list, file_created);

    buffer_free(&file_name_buffer);

    wrefresh(base_window);
}