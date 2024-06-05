#include "aloe/graphic.h"
#include <ncurses.h>



void start_word_counter_popup(file_t* file){
    const int start_x = LINES /2;
    const int start_y = COLS /2;
    
    char* message = "File length data";

    int height = (int)(LINES* 0.34);
    int width = (int)(COLS* 0.254) + (strlen(message) / sizeof(char));
    WINDOW* popup;

    popup = newwin(
        height,
        width, 
        start_x - (height/2),
        start_y - (width/2)
    );

    mvwaddstr(popup, 9, 2, message);

    box(popup, 0, 0);
    wrefresh(popup);
    
    keypad(popup, TRUE);
    nodelay(popup, true);
    cbreak();
    noecho();


    for(;;){}

CLEANING:
    werase(popup);
    wrefresh(popup);
    delwin(popup);
}
