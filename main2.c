#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "aloe/fs.h"
#include "aloe/graphic.h"

int main(int argc, char** argv){
    if(argc == 1){
        return -1;
    }
    char* path = argv[1];

    WINDOW* base_window = setup_base_window();
    file_t main_file = open_file(path);

    
    start_terminal_window(base_window);
    start_file_info_window(base_window,&main_file);
    WINDOW* file_editor_window = start_file_editor_window(base_window);
    WINDOW* termninal_window = start_terminal_window(base_window);
    
    update_file_editor_window(file_editor_window, &main_file, (int)NULL);
    for(;;){
        int input = getch();
        if (input == ERR){
            continue;
        }
        update_file_editor_window(file_editor_window, &main_file, (char)input);
    }


    close_file(&main_file); 
    
    return 0;

}
// Na, ki a vilag legjobb programozoja