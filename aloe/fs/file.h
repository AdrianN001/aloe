#ifndef __ALOE_FILE_H_
#define __ALOE_FILE_H_
#include <stdio.h>

#include "aloe/buffer.h"
#include <stdbool.h>

typedef struct{ 
    FILE* fp;
    complex_buffer_t buffer;

    int row_pointer;
    int collumn_pointer;
    int row_offset;

    bool dirty;
    char* file_name;
} file_t;

file_t open_file(char* path);

int write_to_file(file_t* file);
int delete_from_file(file_t* file);
int save_file(file_t* file);


int close_file(file_t* file);
#endif