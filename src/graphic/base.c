#include "aloe/graphic.h"
#include <stdlib.h>

WINDOW* setup_base_window(void){ 
    WINDOW* main_window;
    if ( (main_window = initscr()) == NULL ) {
	    fprintf(stderr, "Error initialising ncurses.\n");
	    exit(1);
    }
    curs_set(0); /* Hide the cursor */
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    return main_window;
}