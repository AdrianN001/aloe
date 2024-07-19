#include "aloe/graphic.h"
#include <stdlib.h>
#include <locale.h>


WINDOW* setup_base_window(void){ 
    ESCDELAY = 0; // remove delay after pressing escape
    setlocale(LC_ALL, "");


    WINDOW* main_window;
    if ( (main_window = initscr()) == NULL ) {
	    fprintf(stderr, "Error initialising ncurses.\n");
	    exit(1);
    }
    curs_set(0); /* Hide the cursor */
    cbreak();
    noecho();
    return main_window;
}