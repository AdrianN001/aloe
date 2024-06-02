#include "aloe/graphic.h"
#include "aloe/fs.h"
#include "aloe/assert.h"
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

void start_file_search_window_popup(int start_x, int start_y, dir_t* directory, file_t* file_list, char* project_directory){
    file_search_result_t res = {.result = NULL, .n_of_results = 1};
    bool has_been_freed = false;
    size_t result_pointer = 0;
    buffer_t input_buffer = buffer_init();
    char* message = "File finder 15 (ff15)";
    char* start_of_input_field = ">";
    int height = 20;
    int width = 60 + (strlen(message) / sizeof(char));
    WINDOW* popup;

    popup = newwin(
        height,
        width, 
        start_x - (height/2),
        start_y - (width/2)
    );

    mvwaddstr(popup, 9, 2, message);
    mvwaddstr(popup, 10, 2, start_of_input_field);

    box(popup, 0, 0);
    wrefresh(popup);
    
    keypad(popup, TRUE);
    nodelay(popup, true);
    cbreak();
    noecho();

    char ch;


    while ((ch = wgetch(popup))){
        switch(ch){
            case ERR:{
                continue;
            }
            case KEY_ENTER:{
                char* chosen_file = res.result[result_pointer];
                file_list_append(file_list, chosen_file);
                goto CLEANING;
            }
            case KEY_ARROW_UP:{
                if(res.result == NULL) break;
                result_pointer = (result_pointer == 0) ? res.n_of_results -1: result_pointer-1;
                werase(popup);
                box(popup, 0, 0);

                mvwaddstr(popup, 10, 4, input_buffer.data);
                
                mvwaddstr(popup, 9, 2, message);
                mvwaddstr(popup, 10, 2, start_of_input_field);


                render_result_to_popup_page(popup, &res, result_pointer, project_directory);
                wrefresh(popup);
                break;
            }
            case KEY_ARROW_DOWN:{
                if(res.result == NULL) break;
                result_pointer = (result_pointer + 1) % res.n_of_results;
                werase(popup);
                box(popup, 0, 0);

                mvwaddstr(popup, 10, 4, input_buffer.data);
                
                mvwaddstr(popup, 9, 2, message);
                mvwaddstr(popup, 10, 2, start_of_input_field);


                render_result_to_popup_page(popup, &res, result_pointer, project_directory);
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

                mvwaddstr(popup, 10, 4, input_buffer.data);
                
                mvwaddstr(popup, 9, 2, message);
                mvwaddstr(popup, 10, 2, start_of_input_field);
                break;
            }
            default:{
                if(res.result != NULL){
                    free_file_search_result(&res);
                    has_been_freed = true;
                }
                buffer_write_at(&input_buffer, ch, input_buffer.pointer);
                mvwaddstr(popup, 10, 4, input_buffer.data);
                res = search_for_file(directory, input_buffer.data);

                werase(popup);
                box(popup, 0, 0);

                mvwaddstr(popup, 10, 4, input_buffer.data);
                
                mvwaddstr(popup, 9, 2, message);
                mvwaddstr(popup, 10, 2, start_of_input_field);


                render_result_to_popup_page(popup, &res, result_pointer, project_directory);
                has_been_freed = false;
                wrefresh(popup);

                break;
            }
        }
    }

CLEANING:
    werase(popup);
    wrefresh(popup);
    delwin(popup);

    if(!has_been_freed){
        free(res.result);
    }

    return;
}


void render_result_to_popup_page(WINDOW* popup_page, file_search_result_t* result, size_t pointer_position, char* project_directory){

    for(int i = 0; i < result->n_of_results; i++){
        if(i == pointer_position){
            mvwprintw(popup_page, 3+ i, 30, ">%s", result->result[i]);
        }else{
            mvwprintw(popup_page, 3+ i, 30, "%s", result->result[i]);
        }
    }
}