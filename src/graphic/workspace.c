#include "aloe/graphic.h"
#include "aloe/fs.h"
#include "aloe/assert.h"
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

WINDOW* start_workspace_window(WINDOW* base, dir_t* directory){

    WINDOW* workspace_window;

    const int x_top =   (int)(COLS* 0.807);
    const int y_top = 0;
    const int height =  (int)(LINES*0.69);
    const int width  =  (int)(COLS* 0.191);
    
    workspace_window = subwin(base, height, width, y_top, x_top);
    assert(workspace_window != NULL);

    keypad(workspace_window, TRUE);
    box(workspace_window, 0, 0);


    if(directory == NULL){
        const char* info_text = "No directory were opened";
        mvwaddstr(workspace_window, height/2, width/2 - strlen(info_text)/2, info_text);

        wrefresh(workspace_window);
        return workspace_window;
    }
    char* abs_path = realpath( directory->dir_path,NULL);
    mvwprintw(workspace_window, 0, width/2 - strlen(abs_path)/2, "|%s|", abs_path);
    free(abs_path);

    /* Subdirs */

    const char* subdir_text = "------|Subdirectories|------";
    mvwprintw(workspace_window, 4, width/2 - strlen(subdir_text)/2, "%s", subdir_text);

    for (int i = 0; i < directory->n_subdir; i++){
        char* path_without_dir = get_filename_by_path(directory->subdirectories[i].dir_path);
        if ( i == directory->pointer){
            mvwprintw(workspace_window, 5+i, 2, ">%s", path_without_dir);
        }else{
            mvwprintw(workspace_window, 5+i, 2, "%s", path_without_dir);
        }
    }

    mvwprintw(workspace_window, 5+ directory->n_subdir +2, 2, "<-- Go to parent directory"); 
    mvwprintw(workspace_window, 5+ directory->n_subdir +4, 2, "--> Open the directory/file"); 


    /* Files */

    const char* files_text = "------|Files|------";
    mvwprintw(workspace_window, height/2.5+ 5+ directory->n_subdir, width/2 - strlen(files_text)/2, "%s",files_text);

    for (int i = directory->n_subdir; i < directory->n_subdir + directory->n_files; i++){
        char* path_without_dir = get_filename_by_path(directory->files[i - directory->n_subdir]);
         if ( i == directory->pointer){
            mvwprintw(workspace_window, height/2.5 +(4+i+2), 2, ">%s", path_without_dir);
        }else{
            mvwprintw(workspace_window, height/2.5 +(4+i+2), 2, "%s", path_without_dir);
        }
    }



    wrefresh(workspace_window);
    return workspace_window;
}

void update_workspace_window(WINDOW* window,dir_t* directory, file_list_t* file_list,  int character_pressed){

    const int height =  (int)(LINES*0.69);
    const int width  =  (int)(COLS* 0.191);
    

    werase(window);
    box(window, 0,0);



    switch(character_pressed){
        case KEY_ARROW_DOWN:{
            directory->pointer = (directory->pointer + 1) % (directory->n_files + directory->n_subdir );

            break;
        }
        case KEY_ARROW_UP:{
            directory->pointer = (directory->pointer == 0) ? (directory->n_files + directory->n_subdir-1): directory->pointer -1;
            break;
        }
        case KEY_ARROW_LEFT:{
            char* abs_path = realpath( directory->dir_path,NULL);
            char* parent_dir = dirname(abs_path);
            dir_t temp = create_directory_object(parent_dir, 4);
            free(abs_path);

            free_directory_object(directory);
            *directory = temp;

            break;
        }
        case KEY_ARROW_RIGHT:{
            if(directory->pointer < directory->n_subdir){
                /* The pointer is at one of the subdirs */
                char* abs_path = realpath(directory->subdirectories[directory->pointer].dir_path, NULL);
                free_directory_object(directory);
                *directory = create_directory_object(abs_path,3);

                free(abs_path);

            }else if (directory->pointer < (directory->n_subdir + directory->n_files)){
                /* The pointer is at one of the files */
                char* choosen_file = directory->files[directory->pointer -directory->n_subdir];

                char* dir_fullpath = realpath(directory->dir_path, NULL);
                size_t size_fullpath = strlen(dir_fullpath); 

                char full_name[128] = {0}; 
                strcpy(full_name, dir_fullpath);
                free(dir_fullpath);
                
                full_name[size_fullpath] = '/';
                strcpy(&full_name[size_fullpath+1], choosen_file);


                if(file_list_append(file_list, full_name) != NULL){
                    file_list->active_file_pointer = file_list->open_file_n-1;
                    file_list->active_file = &(file_list->open_files[file_list->active_file_pointer]);
                }
            }
            
            break;
        }
    }


    char* abs_path = realpath( directory->dir_path,NULL);
    mvwprintw(window, 0, width/2 - strlen(abs_path)/2, "|%s|", abs_path);
    free(abs_path);

    /* Subdirs */

    const char* subdir_text = "------|Subdirectories|------";
    mvwprintw(window, 4, width/2 - strlen(subdir_text)/2, "%s", subdir_text);

    for (int i = 0; i < directory->n_subdir; i++){
        char* path_without_dir = get_filename_by_path(directory->subdirectories[i].dir_path);
        if ( i == directory->pointer){
            mvwprintw(window, 5+i, 2, ">%s", path_without_dir);
        }else{
            mvwprintw(window, 5+i, 2, "%s", path_without_dir);
        }
    }

    mvwprintw(window, 5+ directory->n_subdir +2, 2, "<-- Go to parent directory"); 
    mvwprintw(window, 5+ directory->n_subdir +4, 2, "--> Open the directory/file"); 


    /* Files */

    const char* files_text = "------|Files|------";
    mvwprintw(window, height/2.5+ 5+ directory->n_subdir, width/2 - strlen(files_text)/2, "%s", files_text);

    for (int i = directory->n_subdir; i < directory->n_subdir + directory->n_files; i++){
        char* path_without_dir = get_filename_by_path(directory->files[i - directory->n_subdir]);
        if ( i == directory->pointer){
            mvwprintw(window, height/2.5 +(4+i+2), 2, ">%s", path_without_dir);
        }else{
            mvwprintw(window, height/2.5 +(4+i+2), 2, "%s", path_without_dir);
        }
    }



    wrefresh(window);
    
}