#include <ncurses.h>
#include <string.h>
#include "aloe/graphic.h"
#include "aloe/assert.h"
#include "aloe/terminal.h"


WINDOW* start_terminal_window(WINDOW* base){
    WINDOW* terminal_window;

    const int x_top = 0;
    const int y_top =   (int)(LINES*0.92);
    const int height =  (int)(LINES*0.1);
    const int width  =  (int)(COLS*0.593);
    
    terminal_window = subwin(base, height, width, y_top, x_top);

    assert(terminal_window != NULL);

    keypad(terminal_window, TRUE);
    box(terminal_window, 0, 0);


    nodelay(terminal_window, true); // Causes getch to be non-blocking

    mvwaddstr(terminal_window, 0, 1, "|Terminal|");
    mvwaddch(terminal_window, height/2, 2, '>');
    wrefresh(terminal_window);

    refresh();
    return terminal_window;
}


void update_terminal_window(WINDOW* window, user_interface_t* ui, int character, buffer_t* buffer, WINDOW* base_window, file_list_t* file_list, dir_t* workspace ){
    const int height =  (int)(LINES*0.1);
    const int width  =  (int)(COLS*0.593);

    for(int i = 0; i < 128; i++){
        mvwaddch(window, height/2, i+3, ' ');
    }
    wrefresh(window);


    switch((char)character){

        case KEY_ARROW_UP:
        case KEY_ARROW_DOWN:
        case KEY_ARROW_LEFT:
        case KEY_ARROW_RIGHT:
        case KEY_TAB:

        case KEY_ENTER:{
            terminal_command_result_t result = try_to_execute_terminal_command(&terminal_command_list, buffer->data, base_window, file_list, workspace);
            if(result.exit_code == 0){
                buffer_clear(buffer);
                file_list_handle_file_events(file_list, workspace);
                user_interface_update_all(ui, file_list, workspace);
            }
            mvwaddstr(window, height/ 2, width/2 -strlen(result.exit_message)/2, result.exit_message );
            wrefresh(window);

            free_terminal_command_result(&result);
            break;
        }
        case KEY_BACKSPACE:{
            buffer_delete_at(buffer, buffer->pointer-1);
            break;
        }
        
        default:{
            buffer_write_at(buffer, character, buffer->pointer);
            break;
        }
    }
    mvwaddstr(window, height/2, 3, buffer->data);
    wrefresh(window);

}