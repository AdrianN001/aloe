#ifndef __FILE_LIST_H_
#define __FILE_LIST_H_


#include "file.h"


#define MAX_FILES_ALLOWED 20

/*

    this struct represents an underlying buffer of opened files

*/
typedef struct{ 

    file_t* active_file;

    file_t* open_files;
    size_t  open_file_n;
} file_list_t;

file_list_t file_list_init(int max_files_n);
void file_list_free(file_list_t* file_list);

void file_list_switch_to(file_list_t* file_list, int index);

/* Tries to close the file; Returns -1 if the file is still dirty */
int file_list_try_close_file(file_list_t* file_list);
/* Closes the file without saving */
void file_list_force_close_file(file_list_t* file_list);
/* Closes the file after it saves */
void file_list_save_and_close_file(file_list_t* file_list);



#endif