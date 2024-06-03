#include <ncurses.h>
#include <string.h>
#include "aloe/graphic.h"
#include "aloe/assert.h"


WINDOW* start_terminal_window(WINDOW* base){
    WINDOW* terminal_window;

    const int x_top = 0;
    const int y_top =   (int)(LINES*0.92);
    const int height =  (int)(LINES*0.1);
    const int width  =  (int)(COLS*0.593);
    
    terminal_window = subwin(base, height, width, y_top, x_top);

    assert(terminal_window != NULL);

    keypad(terminal_window, TRUE);
    box(terminal_window, 0, 0);


    nodelay(terminal_window, true); // Causes getch to be non-blocking

    mvwaddstr(terminal_window, 0, 1, "|Terminal|");
    const char* debug_text = "Still in development.";
    mvwaddstr(terminal_window, height/2, width/2 - strlen(debug_text)/2, debug_text);
    wrefresh(terminal_window);

    refresh();
    return terminal_window;
}
