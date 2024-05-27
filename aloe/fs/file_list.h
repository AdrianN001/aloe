#ifndef __FILE_LIST_H_
#define __FILE_LIST_H_


#include "file.h"


#define MAX_FILES_ALLOWED 20

/*

    this struct represents an underlying buffer of opened files

*/
typedef struct{ 

    file_t* active_file;
    int active_file_pointer;

    file_t* open_files;
    size_t  open_file_n;
} file_list_t;

file_list_t file_list_init();
void file_list_free(file_list_t* file_list);

/* Returns a pointer to the newly opened file */
file_t* file_list_append(file_list_t* file_list, char* path);


void file_list_increment_active_pointer(file_list_t* file_list);
void file_list_decrement_active_pointer(file_list_t* file_list);

/* Tries to close the file; Returns -1 if the file is still dirty */
int file_list_try_close_file(file_list_t* file_list);
/* Closes the file without saving */
void file_list_force_close_file(file_list_t* file_list);
/* Closes the file after it saves */
void file_list_save_and_close_file(file_list_t* file_list);



#endif