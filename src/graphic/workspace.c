#include "aloe/graphic.h"
#include "aloe/fs.h"
#include "aloe/assert.h"
#include <string.h>
#include <stdlib.h>
#include <libgen.h>

WINDOW* start_workspace_window(WINDOW* base, dir_t* directory){

    WINDOW* workspace_window;

    const int x_top = 190;
    const int y_top = 0;
    const int height = 40;
    const int width  = 45;
    
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
    mvwprintw(workspace_window, 4, width/2 - strlen(subdir_text)/2, subdir_text);

    for (int i = 0; i < directory->n_subdir; i++){
        char* path_without_dir = get_filename_by_path(directory->subdirectories[i].dir_path);
        mvwprintw(workspace_window, 5+i, 2, "%s", path_without_dir);
    }

    mvwprintw(workspace_window, 5+ directory->n_subdir +2, 2, "<-- Go to parent directory"); 
    mvwprintw(workspace_window, 5+ directory->n_subdir +4, 2, "--> Open the directory"); 


    /* Files */

    const char* files_text = "------|Files|------";
    mvwprintw(workspace_window, height/2.5+ 5+ directory->n_subdir, width/2 - strlen(files_text)/2, files_text);

    for (int i = directory->n_subdir+2; i < directory->n_subdir + directory->n_files+2; i++){
        char* path_without_dir = get_filename_by_path(directory->files[i - directory->n_subdir-2]);
        mvwprintw(workspace_window, height/2.5 +(4+i), 2, " %s", path_without_dir);
    }



    wrefresh(workspace_window);
    return workspace_window;
}

void update_workspace_window(WINDOW* window, dir_t* directory, int character_pressed){
    const int x_top = 190;
    const int y_top = 0;
    const int height = 40;
    const int width  = 45;

    werase(window);
    box(window, 0,0);



    switch(character_pressed){
        case KEY_ARROW_DOWN:{
            directory->pointer++;
            break;
        }
        case KEY_ARROW_UP:{
            directory->pointer--;
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
            
            break;
        }
    }
    char* abs_path = realpath( directory->dir_path,NULL);
    mvwprintw(window, 0, width/2 - strlen(abs_path)/2, "|%s|", abs_path);
    free(abs_path);

    /* Subdirs */

    const char* subdir_text = "------|Subdirectories|------";
    mvwprintw(window, 4, width/2 - strlen(subdir_text)/2, subdir_text);

    for (int i = 0; i < directory->n_subdir; i++){
        char* path_without_dir = get_filename_by_path(directory->subdirectories[i].dir_path);
        mvwprintw(window, 5+i, 2, "%s", path_without_dir);
    }

    mvwprintw(window, 5+ directory->n_subdir +2, 2, "<-- Go to parent directory"); 
    mvwprintw(window, 5+ directory->n_subdir +4, 2, "--> Open the directory"); 


    /* Files */

    const char* files_text = "------|Files|------";
    mvwprintw(window, height/2.5+ 5+ directory->n_subdir, width/2 - strlen(files_text)/2, files_text);

    for (int i = directory->n_subdir+2; i < directory->n_subdir + directory->n_files+2; i++){
        char* path_without_dir = get_filename_by_path(directory->files[i - directory->n_subdir-2]);
        mvwprintw(window, height/2.5 +(4+i), 2, " %s", path_without_dir);
    }



    wrefresh(window);
    
}