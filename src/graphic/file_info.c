#include "aloe/graphic.h"
#include "aloe/assert.h"
#include "aloe/fs.h"
#include <string.h>

WINDOW* start_file_info_window(WINDOW* base, file_t* file){
    WINDOW* file_info_window;

    const int x_top =   (int)(COLS*0.595);
    const int y_top =   (int)(LINES*0.92);
    const int height =  (int)(LINES*0.1);
    const int width  =  (int)(COLS*0.296);
    
    file_info_window = subwin(base, height, width, y_top, x_top);


    assert(file_info_window != NULL);

    keypad(file_info_window, TRUE);
    box(file_info_window, 0, 0);

    if (file != NULL){
        file_info_t info = get_file_metadata(file);
        mvwaddstr(file_info_window, 1, width /2 - strlen(info.file_name) /2, info.file_name);
        mvwaddstr(file_info_window, 2, width /2 - strlen(info.file_size) /2, info.file_size);
        mvwaddstr(file_info_window, 3, width /2 - strlen(info.file_type) /2, info.file_type);

        mvwprintw(file_info_window, 1, 1, "Owner: %s|", info.file_owner);
    }else{
        const char* no_file_opened = "Open a file.";
        mvwaddstr(file_info_window, height/2, width/2-strlen(no_file_opened)/2,no_file_opened );
    }


    wrefresh(file_info_window);
    return file_info_window;
}

void update_file_info_window(WINDOW* window, file_t* new_file){
    const int height =  (int)(LINES*0.1);
    const int width  =  (int)(COLS*0.296);
    
    werase(window);
    box(window, 0,0);



    if (new_file != NULL){
        file_info_t info = get_file_metadata(new_file);
        mvwaddstr(window, 1, width /2 - strlen(info.file_name) /2, info.file_name);
        mvwaddstr(window, 2, width /2 - strlen(info.file_size) /2, info.file_size);
        mvwaddstr(window, 3, width /2 - strlen(info.file_type) /2, info.file_type);

        mvwprintw(window, 1, 1, "Owner: %s|", info.file_owner);
    }else{
        const char* no_file_opened = "Open a file.";
        mvwaddstr(window, height/2, width/2-strlen(no_file_opened)/2,no_file_opened );
    }


    wrefresh(window);
}