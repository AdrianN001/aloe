

#include <ncurses.h>
#include <stdio.h>
#include <string.h>
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
    werase(window);
    box(window, 0, 0);

    const int row_rendering_offset = 3;
    const int collumn_rendering_offset = 2;

    
    switch(character){
        case (int)NULL:{
            break;
        }
        case KEY_ARROW_UP :{
            file->row_pointer = (file->row_pointer == 0) ? 0 : file->row_pointer -1 ;
            file->collumn_pointer = (file->collumn_pointer > file->buffer.data[file->row_pointer].pointer) ? file->buffer.data[file->row_pointer].pointer : file->collumn_pointer ;  
            break;
        }
        case KEY_ARROW_DOWN:{
            file->row_pointer = (file->buffer.pointer-1 == file->row_pointer) ? file->row_pointer:file->row_pointer + 1;
            file->collumn_pointer = (file->collumn_pointer > file->buffer.data[file->row_pointer].pointer) ? file->buffer.data[file->row_pointer].pointer : file->collumn_pointer ;  
            break;
        }
        case KEY_ARROW_LEFT:{
            file->collumn_pointer = (file->collumn_pointer == 0) ? 0 : file->collumn_pointer -1;
            break;
        }
        case KEY_ARROW_RIGHT:{
            file->collumn_pointer = (file->collumn_pointer == file->buffer.data[file->row_pointer].pointer) ? file->collumn_pointer : file->collumn_pointer + 1;
            break;
        }
        case KEY_ENTER:{
            file->dirty = true;
            complex_buffer_insert_at(&file->buffer, file->row_pointer + 1);
            file->collumn_pointer = 0;
            file->row_pointer++;
            break;
        }
        case KEY_BACKSPACE:{
            file->dirty = true;
            if (file->collumn_pointer == 0){
                if (file->row_pointer != 0){
                    complex_buffer_remove_from(&file->buffer, file->row_pointer); /* Removes the line and merges with the prev one*/
                    file->row_pointer--; /* Jump up one line */
                    file->collumn_pointer = file->buffer.data[file->row_pointer].pointer; /* Jump to the end of the line */

                }
                break;
            }
            delete_from_file(file); /*Deletes one character*/
            file->collumn_pointer = (file->collumn_pointer == 0) ? 0 : file->collumn_pointer -1;
            break;
        }
        case CTRL('a'):{
            save_file(file);
            show_save_popup_window(LINES /2 , COLS /2);
            file->dirty = false;
            break;
        }
        case KEY_TAB:{
            file->dirty = true;
            for (int i = 0; i < 4; i++){
                write_to_file(file, (char)' ');
                file->collumn_pointer++;
            }
            break;
        }

        default:{
            file->dirty = true;
            write_to_file(file, (char)character);
            file->collumn_pointer++;
            break;
        }
    }   
    for (int row = 0; row < buffer->pointer; row++){
        buffer_t* current_row = &(buffer->data[row]);

        mvwprintw(window, row+row_rendering_offset, 0, "%d", row+1);

        for (int collumn = 0; collumn < current_row->pointer; collumn++){
            char character = current_row->data[collumn]; 
            mvwaddch(window, row_rendering_offset+row, collumn_rendering_offset+collumn, character);
            wrefresh(window);
        }        
    }

    /* Cursor position */
    mvwprintw(window, 52, 30, "|Ln %d, Col %d|", file->row_pointer+1, file->collumn_pointer+1);

    /*Cursor*/
    mvwaddch(window, file->row_pointer+row_rendering_offset, file->collumn_pointer+collumn_rendering_offset, '_'| A_REVERSE | A_BLINK);

  

    wrefresh(window);
}

void show_save_popup_window(int start_x, int start_y){
    char* message = "The file was sucessfully saved";
    int height = 3;
    int width = 5 + (strlen(message) / sizeof(char));
    WINDOW* popup;

    popup = newwin(
        height,
        width, 
        start_x - (height/2),
        start_y - (width/2)
    );

    mvwaddstr(popup, 1, 2, message);
    box(popup, 0, 0);
    wrefresh(popup);
    nodelay(popup, true);

    char ch;

    while ((ch = getch()))
        if (ch != ERR)
            break;


    werase(popup);
    wrefresh(popup);
    delwin(popup);

    
}
