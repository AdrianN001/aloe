#include "aloe/graphic.h"
#include "aloe/assert.h"
#include "aloe/buffer.h"
#include <stdbool.h>

void start_command_line(command_list_t* command_list, WINDOW* base_window, file_list_t* file_list, dir_t* directory){
    const int start_x = LINES /2;
    const int start_y = COLS /2;

    buffer_t input_buffer = buffer_init();

    command_list_t filtered_list = {.commands=0};
    bool has_been_freed = false;
    int cursor = 0;
    
    char* start_of_input_field = ">";
    char* message = "Command palette";


    int height = (int)(LINES* 0.74);
    int width = (int)(COLS* 0.354) ;
    WINDOW* popup;

    popup = newwin(
        height,
        width, 
        start_x - (height/2),
        start_y - (width/2)
    );
    assert(popup != (void*)(0));
    mvwaddstr(popup, 1, 2, message);
    mvwaddstr(popup, 2, width/2-10, "[Search for command]");
    box(popup, 0, 0);
    wrefresh(popup);
    
    keypad(popup, TRUE);
    nodelay(popup, true);
    cbreak();
    noecho();

    render_command_results_to_popup(popup, command_list, cursor);


    char ch;
    while ((ch = wgetch(popup))){
        switch(ch){
            case ERR:{
                continue;
            }
            break;
             case KEY_ENTER:{
                if(filtered_list.commands == 0){
                    command_callback_t callback_to_execute = command_list->commands[cursor].callback;
                    callback_to_execute(base_window, file_list, directory);
                }else{
                    command_callback_t callback_to_execute = command_list->commands[cursor].callback;
                    callback_to_execute(base_window, file_list, directory);
                }
                goto CLEANING;
            }
            case KEY_ARROW_UP:{
                if(filtered_list.commands == 0){
                    cursor = (cursor == 0) ? command_list->n_of_commands -1: cursor-1;
                    render_command_results_to_popup(popup, command_list, cursor);
                }else{
                    cursor = (cursor == 0) ? filtered_list.n_of_commands -1: cursor-1;
                    render_command_results_to_popup(popup, filter_commands, cursor);
                }

                wrefresh(popup);
                break;
            }
            case KEY_ARROW_DOWN:{

                if(filtered_list.commands == 0){
                    cursor = (cursor+1) % command_list->n_of_commands;
                    render_command_results_to_popup(popup, command_list, cursor);
                }else{
                    cursor = (cursor+1) % filtered_list.n_of_commands;
                    render_command_results_to_popup(popup, filter_commands, cursor);
                }

                wrefresh(popup);
                break;
            }
            case KEY_ESCAPE:{
                goto CLEANING;
            }
            
            
            case KEY_BACKSPACE:{ /* Backspace */

                buffer_delete_at(&input_buffer, input_buffer.pointer-1);


                werase(popup);
                box(popup, 0, 0);

                if(input_buffer.pointer == 0){
                    mvwaddstr(popup, 2, width/2-10, "[Search for command]");
                }else{
                    mvwprintw(popup, 2, width/2-10, "[%s]", input_buffer.data);
                }
                
                mvwaddstr(popup, 1, 2, message);
               
                break;
            }
            default:{
                if(filtered_list.commands != 0){
                    free_command_list(&filtered_list);
                    has_been_freed = true;
                }
                buffer_write_at(&input_buffer, ch, input_buffer.pointer);
                filtered_list = filter_commands(command_list, input_buffer.data);
                has_been_freed = false;


                werase(popup);
                box(popup, 0, 0);

                mvwprintw(popup, 2, width/2-10, "[%s]", input_buffer.data);
                
                mvwaddstr(popup, 1, 2, message);
                

                render_command_results_to_popup(popup, &filtered_list, cursor);
                wrefresh(popup);

                break;
            }
        }
        
    }

CLEANING:

    werase(popup);
    wrefresh(popup);
    delwin(popup);

    buffer_free(&input_buffer);

    if(!has_been_freed){
        free_command_list(&filtered_list);
    }

    return;
}

void render_command_results_to_popup(WINDOW* popup,command_list_t* command_list,  int cursor){
    for (int i = 0; i < command_list->n_of_commands ; i++){
        if( i == cursor){
            mvwprintw(popup, i*3 + 4, 40, ">%s ", command_list->commands[i].name);
        }else{
            mvwprintw(popup, i*3 + 4, 40, "%s ", command_list->commands[i].name);
        }
        mvwprintw(popup, i*3 + 5, 40, "%s ", command_list->commands[i].description);
    }
}