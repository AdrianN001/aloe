#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>


#include "aloe/fs.h"
#include "aloe/graphic.h"

int main(int argc, char** argv){
    if(argc == 1){
        return -1;
    }
    char* path = argv[1];

    WINDOW* base_window = setup_base_window();
    file_t main_file = open_file(path);

    start_file_info_window(base_window, &main_file);
    WINDOW* file_editor_window = start_file_editor_window(base_window);
    WINDOW* termninal_window = start_terminal_window(base_window);
    WINDOW* time_window = start_time_window(base_window);
    
    update_file_editor_window(file_editor_window, &main_file, (int)NULL);
    for(;;){
        int input = wgetch(file_editor_window);
        if (input == ERR){
            update_time_window(time_window);
            goto SLEEP;
        }
        update_file_editor_window(file_editor_window, &main_file, (char)input);
SLEEP:
        usleep(1 * 10e3);
    }


    close_file(&main_file); 
    
    return 0;
}