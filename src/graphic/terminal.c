#include <ncurses.h>

#include "aloe/graphic.h"
#include "aloe/assert.h"


WINDOW* start_terminal_window(WINDOW* base){
    WINDOW* terminal_window;

    const int x_top = 0;
    const int y_top = 53;
    const int height = 10;
    const int width  = 140;
    
    terminal_window = subwin(base, height, width, y_top, x_top);

    assert(terminal_window != NULL);

    keypad(terminal_window, TRUE);
    box(terminal_window, 0, 0);


    nodelay(terminal_window, true); // Causes getch to be non-blocking

    mvwaddstr(terminal_window, 0, 1, "|Terminal|");
    wrefresh(terminal_window);

    refresh();
    return terminal_window;
}
