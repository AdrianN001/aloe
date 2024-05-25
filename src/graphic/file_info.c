#include "aloe/graphic.h"
#include "aloe/assert.h"
#include "aloe/fs.h"
#include <string.h>

WINDOW* start_file_info_window(WINDOW* base, file_t* file){
    WINDOW* file_info_window;

    const int x_top = 140;
    const int y_top = 53;
    const int height = 5;
    const int width  = 70;
    
    file_info_window = subwin(base, height, width, y_top, x_top);


    assert(file_info_window != NULL);

    keypad(file_info_window, TRUE);
    box(file_info_window, 0, 0);


    file_info_t info = get_file_metadata(file);
    mvwaddstr(file_info_window, 1, width /2 - strlen(info.file_name) /2, info.file_name);
    mvwaddstr(file_info_window, 2, width /2 - strlen(info.file_size) /2, info.file_size);
    mvwaddstr(file_info_window, 3, width /2 - strlen(info.file_type) /2, info.file_type);

    mvwprintw(file_info_window, 1, 1, "Owner: %s|", info.file_owner);



    wrefresh(file_info_window);
    return file_info_window;
}