

#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aloe/buffer.h"
#include "aloe/graphic.h"
#include "aloe/assert.h"

WINDOW* start_file_editor_window(WINDOW* base_window){
    WINDOW* file_editor_window;

    const int x_top = 0;
    const int y_top = 0;
    const int height = (int)(LINES*0.92);
    const int width  = (int)(COLS*0.805);
    
    file_editor_window = subwin(base_window, height, width, y_top, x_top);


    assert(file_editor_window != NULL);

    keypad(file_editor_window, TRUE);
    box(file_editor_window, 0, 0);

    wrefresh(file_editor_window);

    nodelay(file_editor_window, true); // Causes getch to be non-blocking

    return file_editor_window;
}


void update_file_editor_window(WINDOW* window, file_list_t* file_list, int character){
    file_t* active_file = file_list->active_file;
    if(active_file == NULL){
        return;
    }
    complex_buffer_t* buffer = &(active_file->buffer);
    werase(window);
    box(window, 0, 0);

    const int row_rendering_offset = 2;
    const int collumn_rendering_offset = 5;
    const int max_rendered_line = (int)(LINES*0.863);

    
    switch(character){
        case (int)0:{
            break;
        }
        case KEY_ARROW_UP :{
            if(active_file->row_pointer != 0) {
                active_file->row_pointer-- ;
                if (active_file->row_pointer >= max_rendered_line){
                    active_file->row_offset = active_file->row_pointer - max_rendered_line + 5;
                }else if(active_file->row_pointer <= max_rendered_line){
                    active_file->row_offset = 0;
                }
            } 
            active_file->collumn_pointer = (active_file->collumn_pointer > active_file->buffer.data[active_file->row_pointer].pointer) ? active_file->buffer.data[active_file->row_pointer].pointer : active_file->collumn_pointer ;  
            break;
        }
        case KEY_ARROW_DOWN:{
            if (active_file->buffer.pointer-1 != active_file->row_pointer){
                active_file->row_pointer++;
                if (active_file->row_pointer >= max_rendered_line){
                    active_file->row_offset = active_file->row_pointer - max_rendered_line + 5;
                }
            }
            active_file->collumn_pointer = (active_file->collumn_pointer > active_file->buffer.data[active_file->row_pointer].pointer) ? active_file->buffer.data[active_file->row_pointer].pointer : active_file->collumn_pointer ;  
            break;
        }
        case KEY_ARROW_LEFT:{
            active_file->collumn_pointer = (active_file->collumn_pointer == 0) ? 0 : active_file->collumn_pointer -1;
            break;
        }
        case KEY_ARROW_RIGHT:{
            active_file->collumn_pointer = (active_file->collumn_pointer == active_file->buffer.data[active_file->row_pointer].pointer) ? active_file->collumn_pointer : active_file->collumn_pointer + 1;
            break;
        }
        case KEY_ENTER:{
            active_file->dirty = true;
            complex_buffer_insert_at(&active_file->buffer, active_file->row_pointer + 1);
            active_file->collumn_pointer = 0;
            active_file->row_pointer++;
            break;
        }
        case KEY_BACKSPACE:{
            active_file->dirty = true;
            if (active_file->collumn_pointer == 0){
                if (active_file->row_pointer != 0){
                    complex_buffer_remove_from(&active_file->buffer, active_file->row_pointer); /* Removes the line and merges with the prev one*/
                    active_file->row_pointer--; /* Jump up one line */
                    active_file->collumn_pointer = active_file->buffer.data[active_file->row_pointer].pointer; /* Jump to the end of the line */
                }
                break;
            }
            active_file->collumn_pointer = (active_file->collumn_pointer == 0) ? 0 : active_file->collumn_pointer -1;
            delete_from_file(active_file); /*Deletes one character*/
            break;
        }
        case CTRL('a'):{
            save_file(active_file);
            active_file->dirty = false;
            return show_saved_popup_window(LINES /2 , COLS /2);
        }
        case CTRL('x'):{
            if( !file_list->active_file->dirty){
                file_list_force_close_file(file_list);
                return update_file_editor_window(window, file_list, KEY_ARROW_LEFT);
            }
            int result = show_close_popup_window(LINES/2, COLS/2);
            if (result == WANT_TO_SAVE){
                file_list_save_and_close_file(file_list);
                return update_file_editor_window(window, file_list, KEY_ARROW_LEFT);

            }else if (result == DONT_WANT_TO_SAVE){
                file_list_force_close_file(file_list);
                return update_file_editor_window(window, file_list, KEY_ARROW_LEFT);
            }else{
                break;
            }
        }
        case KEY_TAB:{
            active_file->dirty = true;
            for (int i = 0; i < 4; i++){
                write_to_file(active_file, (char)' ');
                active_file->collumn_pointer++;
            }
            break;
        }

        default:{
            active_file->dirty = true;
            write_to_file(active_file, (char)character);
            active_file->collumn_pointer++;
            break;
        }
    }   

    for (int i = 0; i < file_list->open_file_n; i++){
        if (file_list->active_file == &file_list->open_files[i]){
            if(file_list->open_files[i].dirty){
                mvwprintw(window, 0, 20+i*30, "|*%s*|", file_list->open_files[i].file_name);
            }else{
                mvwprintw(window, 0, 20+i*30, "|%s|", file_list->open_files[i].file_name);
            }
        }else{
            if(file_list->open_files[i].dirty){
                mvwprintw(window, 0, 20+i*30, "*%s*", file_list->open_files[i].file_name);
            }else{
                mvwprintw(window, 0, 20+i*30, "%s", file_list->open_files[i].file_name);
            }
        }
    }

    bool cursor_added = false;
    for (int row = 0; row < buffer->pointer && row < max_rendered_line; row++){
        buffer_t* current_row = &(buffer->data[row+active_file->row_offset]);

        mvwprintw(window, row+row_rendering_offset, 0, "|%02d|", row+active_file->row_offset+1);

        for (int collumn = 0; collumn < current_row->pointer; collumn++){
            char character = current_row->data[collumn]; 
            if (row == active_file->row_pointer-active_file->row_offset && collumn == active_file->collumn_pointer){
                cursor_added = true;

                mvwaddch(window, row_rendering_offset+row, collumn_rendering_offset+collumn, character | A_REVERSE);
                
                continue;
            }
            mvwaddch(window, row_rendering_offset+row, collumn_rendering_offset+collumn, character);
            wrefresh(window);
        }        
    }

    /* Cursor position */
    mvwprintw(window, 52, 30, "|Ln %d, Col %d|", active_file->row_pointer+1, active_file->collumn_pointer+1);

    // /* Cursor */
    if (!cursor_added)
        mvwaddch(window, active_file->row_pointer+row_rendering_offset, active_file->collumn_pointer+collumn_rendering_offset, '_' |A_REVERSE);
  

    wrefresh(window);

}

void show_saved_popup_window(int start_x, int start_y){
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

    while ((ch = wgetch(popup))) 
        if (ch != ERR)
            break;


    werase(popup);
    wrefresh(popup);
    delwin(popup);
}



int show_close_popup_window(int start_x, int start_y){
    char* message = "The file of yours is dirty. Would you like to save it? ";
    char* confirm_text = "[Y]es/[N]o        [C]ancel";
    int height = 5;
    int width = 5 + (strlen(message) / sizeof(char));
    WINDOW* popup;

    popup = newwin(
        height,
        width, 
        start_x - (height/2),
        start_y - (width/2)
    );

    mvwaddstr(popup, 1, 2, message);
    mvwaddstr(popup, 3, 2, confirm_text);
    box(popup, 0, 0);
    wrefresh(popup);
    nodelay(popup, true);

    char ch;

    int result = 0; 

    while ((ch = getch()))
        switch(ch){
            case ERR:{
                continue;
            }
            case 'y':{
                result = WANT_TO_SAVE;
                goto CLEANING;
            }
            case 'n':{
                result = DONT_WANT_TO_SAVE;
                goto CLEANING;
            }
            case 'c':{
                result = CANCEL;
                goto CLEANING;
            }
        }

CLEANING:
    werase(popup);
    wrefresh(popup);
    delwin(popup);

    return result;
}

buffer_t show_new_file_save_window(int start_x, int start_y){
    buffer_t input_buffer = buffer_init();
    char* message = "What should be the name of the file? ";
    char* start_of_input_field = ">";
    int height = 5;
    int width = 10 + (strlen(message) / sizeof(char));
    WINDOW* popup;

    popup = newwin(
        height,
        width, 
        start_x - (height/2),
        start_y - (width/2)
    );

WRITING:
    mvwaddstr(popup, 1, 2, message);
    mvwaddstr(popup, 2, 2, start_of_input_field);

    box(popup, 0, 0);
    wrefresh(popup);
    nodelay(popup, true);

    int ch;


    while ((ch = wgetch(popup))){
        switch(ch){
            case ERR:{
                continue;
            }
            case KEY_ENTER:{
                goto CLEANING;
            }
            case 127:{
                buffer_delete_at(&input_buffer, input_buffer.pointer-1);
                werase(popup);
                mvwaddstr(popup, 2, 4, input_buffer.data);
                goto WRITING;
                break;
            }
            default:{
                buffer_write_at(&input_buffer, ch, input_buffer.pointer);
                mvwaddstr(popup, 2, 4, input_buffer.data);

                break;
            }
        }
        wrefresh(popup);
    }

CLEANING:
    werase(popup);
    wrefresh(popup);
    delwin(popup);

    return input_buffer;
}