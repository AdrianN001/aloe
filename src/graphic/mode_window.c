#include "aloe/graphic.h"
#include "aloe/assert.h"
#include "aloe/fs.h"
#include <string.h>

static const char* normal_mode_text     = "NORMAL MODE";
static const char* file_edit_mode_text  = "EDIT MODE";
static const char* terminal_mode_text   = "TERMINAL MODE";
static const char* workspace_mode_text  = "WORKSPACE MODE";


WINDOW* start_mode_window(WINDOW* base){
    WINDOW* mode_window;

    const int x_top = 190;
    const int y_top = 40;
    const int height = 5;
    const int width  = 45;
    
    mode_window = subwin(base, height, width, y_top, x_top);


    assert(mode_window != NULL);

    keypad(mode_window, TRUE);
    box(mode_window, 0, 0);

    mvwaddstr(mode_window, height/2, width/2 - strlen(normal_mode_text)/2, normal_mode_text);


    return mode_window;
}

void update_mode_window(WINDOW* window, int current_mode){
    const int x_top = 190;
    const int y_top = 40;
    const int height = 5;
    const int width  = 45;
    werase(window);
    box(window, 0, 0);


    switch(current_mode){
        case BASE_MODE:{
            mvwaddstr(window, height/2, width/2 - strlen(normal_mode_text)/2, normal_mode_text);
            break;
        }
        case TEXT_EDITOR_MODE:{
            mvwaddstr(window, height/2, width/2 - strlen(file_edit_mode_text)/2, file_edit_mode_text);
            break;
        }
        case TERMINAL_MODE:{
            mvwaddstr(window, height/2, width/2 - strlen(terminal_mode_text)/2, terminal_mode_text);
            break;
        }
        case WORKSPACE_MODE:{
            mvwaddstr(window, height/2, width/2 - strlen(workspace_mode_text)/2, workspace_mode_text);
            break;
        }
    }
    wrefresh(window);
}