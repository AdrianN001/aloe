#ifndef __TIME_WINDOW_H_
#define __TIME_WINDOW_H_


#include <ncurses.h>

WINDOW* start_time_window(WINDOW* base);

void update_time_window(WINDOW* window);

//loads the time in format HH-MM-SS into the buffer 
int fetch_current_time(char* buffer);
// loads the date in format YYYY-MM-DD into buffer
int fetch_current_date(char* buffer);
//loads the time in format HH-MM into the buffer 
int fetch_current_time_h_m(char* buffer);  

#endif