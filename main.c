#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>


#include "aloe/fs.h"
#include "aloe/graphic.h"

bool handle_mode_state_change_in_base_mode(int input_character, int* current_state);

int main(int argc, char** argv){
    if(argc == 1){
        return -1;
    }
    char* path = argv[1];
    int type_of_path;
    if ((type_of_path = lookup_path(path)) == INVALID){
        return -1;
    }

    dir_t workspace;
    file_list_t file_list = file_list_init();
    file_t* main_file = NULL;

    if(type_of_path == VALID_DIRECTORY){
        workspace = create_directory_object(path, 4);
    }else if(type_of_path == VALID_FILE){
        main_file = file_list_append(&file_list, path);
    }
    (void)file_list_append(&file_list, "src/fs/file_list.c");
    (void)file_list_append(&file_list, "aloe/buffer.h");
    (void)file_list_append(&file_list, "aloe/graphic.h");
    (void)file_list_append(&file_list, "aloe/fs.h");

    int current_mode = BASE_MODE;


    WINDOW* base_window = setup_base_window();
    WINDOW* workspace_window = start_workspace_window(base_window, type_of_path == VALID_DIRECTORY ? &workspace : NULL);


    WINDOW* mode_window = start_mode_window(base_window);
    WINDOW* file_info_window = start_file_info_window(base_window, file_list.active_file);
    WINDOW* file_editor_window = start_file_editor_window(base_window);
    WINDOW* termninal_window = start_terminal_window(base_window);
    WINDOW* time_window = start_time_window(base_window);
    
    update_file_editor_window(file_editor_window, &file_list, (int)NULL);

    for(;;){
        int input = wgetch(file_editor_window);
        
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
            }
        }

        /* If there is an input and the mode is not "base_mode", but it is the key to go back to base mode  */
        if ( input == BASE_MODE_KEY ){
            update_mode_window(mode_window, BASE_MODE);
            current_mode = BASE_MODE;
            goto SLEEP;
        }

        /* No update needed if there is no input */
        if (input == ERR){
            update_time_window(time_window);
            goto SLEEP;
        }

        switch(current_mode){
            case TEXT_EDITOR_MODE:{
                update_file_editor_window(file_editor_window,&file_list, (char)input);
                update_file_info_window(file_info_window, file_list.active_file);
                break;
            }
            case TERMINAL_MODE:{
                break;
            }
            case WORKSPACE_MODE:{
                update_workspace_window(workspace_window, &workspace, (char)input );
                break;
            }
        }

        

SLEEP:
        usleep(9 * 10e2);
    }

    close_file(main_file); 
    if(type_of_path == VALID_DIRECTORY){
        free_directory_object(&workspace);
    }
    
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