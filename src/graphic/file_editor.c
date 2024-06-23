#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "aloe/buffer.h"
#include "aloe/graphic.h"
#include "aloe/assert.h"
#include "aloe/fs.h"

static int FILE_EDITOR_HEIGHT = 0;
static int FILE_EDITOR_WIDTH = 0;


const static int row_rendering_offset = 2;
const static int collumn_rendering_offset = 6;


WINDOW* start_file_editor_window(WINDOW* base_window){
    WINDOW* file_editor_window;

    const int x_top = 0;
    const int y_top = 0;
    FILE_EDITOR_HEIGHT = (int)(LINES*0.92);
    FILE_EDITOR_WIDTH  = (int)(COLS*0.805);
    
    file_editor_window = subwin(base_window, FILE_EDITOR_HEIGHT, FILE_EDITOR_WIDTH, y_top, x_top);


    assert(file_editor_window != NULL);

    keypad(file_editor_window, TRUE);
    box(file_editor_window, 0, 0);

    wrefresh(file_editor_window);

    nodelay(file_editor_window, true); // Causes getch to be non-blocking

    return file_editor_window;
}


void update_file_editor_window(user_interface_t* ui, file_list_t* file_list, int character){
    file_t* active_file = file_list->active_file;
    if(active_file == NULL){
        render_blank_screen(ui->text_editor_window);
        return;
    }

    const int max_rendered_line = (int)(LINES*0.863);
    const int max_char_in_a_line = FILE_EDITOR_WIDTH -10;

    
    switch(character){
        case (int)0:{
            break;
        }
        case KEY_ARROW_UP :{
            if(active_file->row_pointer != 0) {
                active_file->row_pointer-- ;
                if (active_file->row_pointer == active_file->row_offset && active_file->row_offset != 0){
                    active_file->row_offset--;
                }
            } 
            active_file->collumn_pointer = (active_file->collumn_pointer > active_file->buffer.data[active_file->row_pointer].pointer) ? active_file->buffer.data[active_file->row_pointer].pointer : active_file->collumn_pointer ;  
            wrefresh(ui->text_editor_window);
            break;

        }
        case KEY_ARROW_DOWN:{
            if (active_file->buffer.pointer-1 != active_file->row_pointer){
                active_file->row_pointer++;
                if (active_file->row_pointer == active_file->row_offset+ max_rendered_line && active_file->row_offset != active_file->buffer.pointer){
                    active_file->row_offset++;
                }
            }
            active_file->collumn_pointer = (active_file->collumn_pointer > active_file->buffer.data[active_file->row_pointer].pointer) ? active_file->buffer.data[active_file->row_pointer].pointer : active_file->collumn_pointer ;  
            wrefresh(ui->text_editor_window);

           
            break;
        }
        case KEY_ARROW_LEFT:{
            if (active_file->collumn_pointer == 0){
                active_file->collumn_offset = 0;
                return update_file_editor_window(ui, file_list, (char)KEY_UP);
            }else if ( active_file->collumn_pointer == active_file->collumn_offset && active_file->collumn_offset != 0){
                active_file->collumn_offset--;
            }
            active_file->collumn_pointer--;
            
            break;
        }
        case KEY_ARROW_RIGHT:{
            if (active_file->collumn_pointer+1 == active_file->buffer.data[active_file->row_pointer].pointer){
                active_file->collumn_offset = 0;
                return update_file_editor_window(ui, file_list, (char)KEY_DOWN);
            }else if ( active_file->collumn_pointer == active_file->collumn_offset + max_char_in_a_line-1 && active_file->collumn_offset != active_file->buffer.data[active_file->row_pointer].pointer){
                active_file->collumn_offset++;
            }
            active_file->collumn_pointer++;
            break;
        }
        case KEY_ENTER:{
            insert_new_line_to_file(file_list->active_file);
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
                return update_file_editor_window(ui, file_list, KEY_ARROW_LEFT);
            }
            int result = show_close_popup_window(LINES/2, COLS/2);
            if (result == WANT_TO_SAVE){
                file_list_save_and_close_file(file_list);
                return update_file_editor_window(ui, file_list, KEY_ARROW_LEFT);

            }else if (result == DONT_WANT_TO_SAVE){
                file_list_force_close_file(file_list);
                return update_file_editor_window(ui, file_list, KEY_ARROW_LEFT);
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
    /* Draw the list of the files */
    render_file_list(file_list, ui->text_editor_window);
    render_current_mode(ui->text_editor_window, ui->current_mode);

    render_file_content_to_window(ui->text_editor_window, active_file, max_rendered_line, max_char_in_a_line);
    /* Add cursor */
    char cursor_character = mvwinch(ui->text_editor_window,active_file->row_pointer+row_rendering_offset - active_file->row_offset, active_file->collumn_pointer+collumn_rendering_offset - active_file->collumn_offset );
    mvwaddch(ui->text_editor_window, active_file->row_pointer+row_rendering_offset - active_file->row_offset, active_file->collumn_pointer+collumn_rendering_offset - active_file->collumn_offset, cursor_character|A_REVERSE);


    /* Cursor position */
    mvwprintw(ui->text_editor_window, FILE_EDITOR_HEIGHT-1, FILE_EDITOR_WIDTH-20, "|Ln %d, Col %d|", active_file->row_pointer+1, active_file->collumn_pointer+1);


    wrefresh(ui->text_editor_window);

}

void render_file_content_to_window(WINDOW* text_editor_window,file_t *active_file, const int max_rendered_line, const int max_char_in_a_line){
    for (int row = row_rendering_offset; row < active_file->buffer.pointer + row_rendering_offset && row < max_rendered_line + row_rendering_offset; row++){
        buffer_t *current_row = &(active_file->buffer.data[row - (row_rendering_offset) + active_file->row_offset]);
        mvwprintw(text_editor_window, row, 0, "|%03d|", row + active_file->row_offset + 1 - row_rendering_offset);

        for (int i = collumn_rendering_offset; i < max_char_in_a_line; i++){
            if (i - collumn_rendering_offset < current_row->pointer){
                char current_character = current_row->data[i - collumn_rendering_offset];
                mvwaddch(text_editor_window, row, i, current_character);
            }else{
                mvwaddch(text_editor_window, row, i, ' ');
            }
        }
    }
}
void render_file_list(file_list_t *file_list, WINDOW *window)
{
    for (int i = 0; i < file_list->open_file_n; i++){
        if (file_list->active_file == &file_list->open_files[i]){
            if (file_list->open_files[i].dirty){
                mvwprintw(window, 0, 20 + i * 30, "|*%s*|", file_list->open_files[i].file_name);
            }
            else{
                mvwprintw(window, 0, 20 + i * 30, "|%s|", file_list->open_files[i].file_name);
            }
        }
        else{
            if (file_list->open_files[i].dirty){
                mvwprintw(window, 0, 20 + i * 30, "*%s*", file_list->open_files[i].file_name);
            }
            else{
                mvwprintw(window, 0, 20 + i * 30, "%s", file_list->open_files[i].file_name);
            }
        }
    }
}

void render_current_mode(WINDOW* window, aloe_mode_t mode){
    
    char* mode_texts[] = {
        "  | Normal |  ",
        "  | Editing |  ",
        "  | Terminal |  ",
        "  | Workspace |  "
    };
    mvwaddstr(window,  FILE_EDITOR_HEIGHT-1, 40-strlen(mode_texts[mode])/2,  mode_texts[mode]);
    wrefresh(window);
}

void show_saved_popup_window(int start_x, int start_y)
{
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
            case KEY_ESCAPE:{
                input_buffer.pointer = -1;
                goto CLEANING;
            }
            case 127:{ /* backspace */
                buffer_delete_at(&input_buffer, input_buffer.pointer-1);
                werase(popup);
                mvwaddstr(popup, 2, 4, input_buffer.data);
                goto WRITING;
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


void render_blank_screen(WINDOW* window){

    werase(window);
    box(window, 1,0);

    const char github_message[] = "Aloe";
    mvwaddstr(window, FILE_EDITOR_HEIGHT-2, FILE_EDITOR_WIDTH - sizeof(github_message), github_message);


    char* icon[] = {
        "⠀⠀⠀⢀⣴⣾⣦⣀⣀⣠⣿⣿⣷⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⠈⢻⣿⣿⣿⣿⣿⣿⣿⣧⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠀⠀⢀⣾⣿⡿⠋⠁⠈⠙⢿⣿⣷⣶⣶⡆⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⢸⣿⣿⣿⣿⡇⠀⠀⠀⠀⢸⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀",
        "⠀⠘⠛⠛⠻⣿⣷⣤⣀⣀⣴⣿⣿⠏⢀⣀⠀⠀⠀⠀⣾⣿⣿⡇⠀⠀⠀⠀⣀⠀",
        "⠀⠀⠀⠀⠀⣾⣿⣿⡿⠿⢿⣿⣿⣷⣿⣿⣧⠀⣀⣀⣿⣿⣿⣇⣀⡀⠀⣼⣿⠀",
        "⠀⠀⠀⠀⠸⠿⣿⡿⠀⠀⠀⠻⠿⠋⢻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠁⢀⣴⣤⣀⢀⣴⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠺⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠿⠿⠿⣿⣿⣿⣿⣿⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣾⣿⣿⣿⣿⣿⣿⣿⡟⠀⠀⠀⠀⠀⠈⢻⣿⣿⣿⠀",
        "⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⠀",
        "⠀⠀⠀⠀⠀⠀⠘⠛⠛⠻⣿⣿⣿⣿⣿⣿⣿⣿⡄⠀⠀⠀⠀⠀⠀⣠⣿⣿⣿⠀",
        "⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⠛⠛⠛⠛⠛⠛⠛⠛⠂⠀⠀⠀⠀⠒⠛⠛⠛⠛⠀",
    };

     for (int i = 0; i < 14; i++){
        mvwaddstr(window, i + 10, FILE_EDITOR_WIDTH/2 - 91/5, icon[i]);
    }
    wrefresh(window);



    char* tool_tips[] = {
        "                              ",
        "        <Tool tips>:          ",
        "                              ",
        "                              ",
        "  Ctrl+N => New File          ",
        "  Ctrl+F => Search for file   ",
        "  Ctrl+P => Command line      ",
        "                              ",
        "          <Modes>:            ",
        "                              ",
        " (a)       Text Editor        ",
        " (t)       Terminal           ",
        " (w)       Workspace          ",
        " (esc)     Normal             ",
        "                              ",
        "                              ",
    };
    const size_t row_size = strlen(tool_tips[0]);

    const size_t n_rows = 16;

    for (int i = 0; i < n_rows; i++){
        mvwaddstr(window, i + 30, FILE_EDITOR_WIDTH/2 - row_size/2, tool_tips[i]);
    }
    wrefresh(window);
}
