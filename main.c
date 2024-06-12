#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>
#include <signal.h>
#include <sys/inotify.h>
#include <locale.h>

#include "aloe/fs.h"
#include "aloe/graphic.h"
#include "aloe/assert.h"
#include "aloe/commander.h"
#include "aloe/terminal.h"

static volatile int keep_running = 1;

bool handle_mode_state_change_in_base_mode(int input_character, int* current_state);
void interruptHandler(int);

int main(int argc, char** argv){
    ESCDELAY = 0; // remove delay after pressing escape

    setlocale(LC_ALL, "");

    WINDOW* base_window = NULL;
    file_t file_created;
    int type_of_path = -1;
    char* path;


    if(argc == 1){
        base_window = setup_base_window();
        buffer_t file_name_buffer = show_new_file_save_window(LINES/2,  COLS/2);
        if (file_name_buffer.pointer == -1){
            buffer_free(&file_name_buffer);
            goto CLEANING;
        }
        char* file_name = file_name_buffer.data;

        file_created = create_new_file(file_name);
        path = file_created.absolute_file_name;
        buffer_free(&file_name_buffer);
        type_of_path = NO_FILE_WAS_GIVEN;
    }else{
        path = argv[1];
        if ((type_of_path = lookup_path(path)) == INVALID){
            return -1;
        }
    }

    command_list_t command_list = init_commands();
    initilize_terminal_command_list();

    initilialize_inotify_instance();
    setup_non_blocking_mode(inotify_file_descriptor);


    signal(SIGINT, interruptHandler);


    dir_t workspace;
    file_list_t file_list = file_list_init();

    if(type_of_path == VALID_DIRECTORY){
        workspace = create_directory_object(path, 4);
    }else if(type_of_path == VALID_FILE){
        file_list_append(&file_list, path);
    }else if(type_of_path == NO_FILE_WAS_GIVEN){
        file_list_append_premade(&file_list, file_created);
    }

    int current_mode = BASE_MODE;

    base_window = base_window == NULL ? setup_base_window(): base_window;
    WINDOW* workspace_window = start_workspace_window(base_window, type_of_path == VALID_DIRECTORY ? &workspace : NULL);
    WINDOW* mode_window = start_mode_window(base_window);
    WINDOW* file_info_window = start_file_info_window(base_window, file_list.active_file);
    WINDOW* file_editor_window = start_file_editor_window(base_window);
    WINDOW* termninal_window = start_terminal_window(base_window);
    WINDOW* time_window = start_time_window(base_window);

    buffer_t terminal_buffer = buffer_init();
    
    update_file_editor_window(file_editor_window, &file_list, (int)NULL);

    for(;keep_running;){
        int input = wgetch(file_editor_window);
        
        /* No update needed if there is no input */
        if (input == ERR){
            update_time_window(time_window);
            goto SLEEP;
        }
        
        /* If there is an input and the mode is "base_mode", than handle it  */
        if (current_mode == BASE_MODE){
            bool successfully_handled = handle_mode_state_change_in_base_mode(input, &current_mode);
            if (successfully_handled){
                update_mode_window(mode_window, current_mode);
                goto SLEEP;
            }

            switch((char)input){
                case KEY_ARROW_LEFT:{
                    file_list_decrement_active_pointer(&file_list);
                    update_file_editor_window(file_editor_window,&file_list, (char)input);
                    update_file_info_window(file_info_window, file_list.active_file);

                    break;
                }
                case KEY_ARROW_RIGHT:{
                    file_list_increment_active_pointer(&file_list);
                    update_file_editor_window(file_editor_window,&file_list, (char)input);
                    update_file_info_window(file_info_window, file_list.active_file);

                    break;
                }
                case CTRL('a'):{
                    save_file(file_list.active_file);
                    file_list.active_file->dirty = false;
                    show_saved_popup_window(LINES /2 , COLS /2);

                    update_file_editor_window(file_editor_window,&file_list, (int)NULL);
                    break;
                }
                case CTRL('p'):{
                    start_command_line(&command_list, base_window, &file_list, &workspace);
                    break;
                }
                default:{
                    /* Execute shortcuts */
                    for(int i = 0; i < command_list.n_of_commands; i++){
                        if(input == command_list.commands[i].shortcut){
                            command_callback_t callback = command_list.commands[i].callback;
                            callback(base_window, &file_list, &workspace);
                            update_file_editor_window(file_editor_window, &file_list, (int)NULL);
                            break;
                        }
                    }
                    break;
                }
            }
        }

        /* If there is an input and the mode is not "base_mode", but it is the key to go back to base mode  */
        if ( input == BASE_MODE_KEY ){
            update_mode_window(mode_window, BASE_MODE);
            current_mode = BASE_MODE;
            goto SLEEP;
        }


        switch(current_mode){
            case TEXT_EDITOR_MODE:{
                update_file_editor_window(file_editor_window,&file_list, (char)input);
                update_file_info_window(file_info_window, file_list.active_file);
                break;
            }
            case TERMINAL_MODE:{
                update_terminal_window(termninal_window, input, &terminal_buffer, base_window, &file_list, &workspace);
                break;
            }
            case WORKSPACE_MODE:{
                update_workspace_window(workspace_window, &workspace, &file_list, (char)input );
                update_file_editor_window(file_editor_window,&file_list, (int)NULL);
                update_file_info_window(file_info_window, file_list.active_file);
                break;
            }
        }

        
//  asd asd asd asd asd asd asd asd asd asd asd asd asd asd asd asd asd
SLEEP:
        file_list_handle_file_events(&file_list);
        usleep(9 * 10e2);
    }
CLEANING:

    werase(base_window);
    wrefresh(base_window);
    delwin(base_window);
    endwin();
    free_command_list(&command_list);
    deinitalize_terminal_command_list();
    deinitilialize_inotify_instance();

    file_list_close_all(&file_list);
    file_list_free(&file_list);
    if(type_of_path == VALID_DIRECTORY){
        free_directory_object(&workspace);
    }
    printf("Program exited successfully. Goodbye && Rest a little bit.\n");
    
    return 0;

}




bool handle_mode_state_change_in_base_mode(int input_character, int* current_state){
    if (input_character == TEXT_EDITOR_MODE_KEY){
        *current_state = TEXT_EDITOR_MODE;
        return true;
    }else if (input_character == TERMINAL_MODE_KEY){
        *current_state = TERMINAL_MODE;
        return true;
    }else if (input_character == WORKSPACE_MODE_KEY){
        *current_state = WORKSPACE_MODE;
        return true;
    }
    return false;
}

void interruptHandler(int _x){
    keep_running = 0;
}