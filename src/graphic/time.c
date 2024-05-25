#include "aloe/graphic.h"
#include "aloe/assert.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>


WINDOW* start_time_window(WINDOW* base){
    WINDOW* time_window;

    const int x_top = 210;
    const int y_top = 53;
    const int height = 5;
    const int width  = 26;
    
    time_window = subwin(base, height, width, y_top, x_top);


    assert(time_window != NULL);

    keypad(time_window, TRUE);
    box(time_window, 0, 0);
    
    char render_buffer[20] = {0};
    size_t chars_written = fetch_current_date(render_buffer);
    mvwaddstr(time_window, height/2 + 1, (width/2 - chars_written/2), render_buffer);


    chars_written = fetch_current_time(render_buffer);
    mvwaddstr(time_window, height/2 - 1, (width/2 - chars_written/2), render_buffer);


    wrefresh(time_window);
    return time_window;
}

void update_time_window(WINDOW* window){
    werase(window);
    box(window, 0, 0);


    const int height = 5;
    const int width  = 26;

    char render_buffer[20] = {0};
    size_t chars_written = fetch_current_date(render_buffer);
    mvwaddstr(window, height/2 + 1, (width/2 - chars_written/2), render_buffer);


    chars_written = fetch_current_time(render_buffer);
    mvwaddstr(window, height/2 - 1, (width/2 - chars_written/2), render_buffer);


    wrefresh(window);
}


int fetch_current_time(char* buffer){
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return sprintf(buffer, " %02d:%02d:%02d ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}
    

int fetch_current_date(char* buffer){
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);

    return sprintf(buffer, "%d. %02d. %02d",timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday);
}

int fetch_current_time_h_m(char* buffer){
    time_t rawtime;
    struct tm* timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    return sprintf(buffer, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
}