

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "aloe/graphic.h"
#include "aloe/assert.h"

WINDOW* start_file_editor_window(WINDOW* base_window){
    WINDOW* file_editor_window;

    const int x_top = 0;
    const int y_top = 0;
    const int height = 53;
    const int width  = 190;
    
    file_editor_window = subwin(base_window, height, width, y_top, x_top);


    assert(file_editor_window != NULL);

    keypad(file_editor_window, TRUE);
    box(file_editor_window, 0, 0);

    wrefresh(file_editor_window);

    nodelay(file_editor_window, true); // Causes getch to be non-blocking

    return file_editor_window;
}


void update_file_editor_window(WINDOW* window, file_t* file, int character){
    complex_buffer_t* buffer = &(file->buffer);
    wclear(window);
    box(window, 0, 0);

    const int row_rendering_offset = 1;
    const int collumn_rendering_offset = 2;

    
    switch(character){
        case KEY_ARROW_UP :{
            file->row_pointer = (file->row_pointer == 0) ? 0 : file->row_pointer -1 ;
            break;
        }
        case KEY_ARROW_DOWN:{
            file->row_pointer = (file->buffer.pointer-1 == file->row_pointer) ? file->row_pointer:file->row_pointer + 1;
            break;
        }
        case KEY_ARROW_LEFT:{
            file->collumn_pointer = (file->collumn_pointer == 0) ? 0 : file->collumn_pointer -1;
            break;
        }
        case KEY_ARROW_RIGHT:{
            file->collumn_pointer++;
            break;
        }
    }

    for (int row = 0; row < buffer->pointer; row++){
        buffer_t* current_row = &(buffer->data[row]);
        for (int collumn = 0; collumn < current_row->pointer; collumn++){
            char character = current_row->data[collumn]; 
            mvwaddch(window, row_rendering_offset+row, collumn_rendering_offset+collumn, character);
            wrefresh(window);
        }        
    }
    char position_buffer[21] = {0};
    sprintf(position_buffer, "|Ln %d, Col %d|", file->row_pointer, file->collumn_pointer);
    mvwaddstr(window, 52, 30, position_buffer);

    mvwaddch(window, file->row_pointer+row_rendering_offset, file->collumn_pointer+collumn_rendering_offset, '_'| A_REVERSE | A_BLINK);
    wrefresh(window);
}