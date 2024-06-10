
#include "aloe/fs.h"
#include <string.h>
#include <stdlib.h>
#include "aloe/assert.h"

#ifndef MAX
#define MAX(x, y) ((x < y) ? y : x)
#endif

file_list_t file_list_init(){
    file_t* data = calloc(sizeof(file_t), MAX_FILES_ALLOWED);
    return (file_list_t){
        .active_file = NULL,
        .open_files = data,
        .active_file_pointer = 0,
        .open_file_n = 0
    };
}

void file_list_free(file_list_t* file_list){
    free(file_list->open_files);
    file_list->active_file = NULL;
    file_list->open_file_n = 0;
}

file_t* file_list_append(file_list_t* file_list, char* path){
    if( file_list->open_file_n >= MAX_FILES_ALLOWED){
        return NULL;
    }
    // Check if the file was opened previously
    for ( int i = 0; i < file_list->open_file_n; i++){
        if (strcmp(path, file_list->open_files[i].absolute_file_name) == 0){
            return NULL;
        }
    }

    file_list->open_files[file_list->open_file_n] = open_file(path);
    if (file_list->active_file == NULL ){
        file_list->active_file = &file_list->open_files[file_list->open_file_n];
    }
    return &file_list->open_files[file_list->open_file_n++];
}

file_t* file_list_append_premade(file_list_t* file_list, file_t file){
    if( file_list->open_file_n >= MAX_FILES_ALLOWED){
        return NULL;
    }

    file_list->open_files[file_list->open_file_n] = file;
    if (file_list->active_file == NULL ){
        file_list->active_file = &file_list->open_files[file_list->open_file_n];
    }
    return &file_list->open_files[file_list->open_file_n++];
}

void file_list_increment_active_pointer(file_list_t* file_list){

    file_list->active_file_pointer = (file_list->active_file_pointer + 1) % file_list->open_file_n;
    file_list->active_file = &file_list->open_files[file_list->active_file_pointer];
}

void file_list_decrement_active_pointer(file_list_t* file_list){

    file_list->active_file_pointer = (file_list->active_file_pointer == 0) ? file_list->open_file_n -1: file_list->active_file_pointer - 1;

    file_list->active_file = &file_list->open_files[file_list->active_file_pointer];
}



int file_list_try_close_file(file_list_t* file_list){
    if (file_list->active_file->dirty){
        return -1;
    }
    file_list_force_close_file(file_list);
    return 0;
}

void file_list_force_close_file(file_list_t* file_list){
    close_file(file_list->active_file);
    if(file_list->active_file == &file_list->open_files[file_list->open_file_n-1]){
        file_list->open_file_n = MAX(file_list->open_file_n -1, 0) ;
        file_list->active_file_pointer = MAX(file_list->active_file_pointer -1, 0);
        file_list->active_file = (file_list->open_file_n == 0 )? NULL : &file_list->open_files[file_list->active_file_pointer];
    }
    else{
        /* Pointer arithmetic <3 */
        for (file_t* i = file_list->active_file; i != &file_list->open_files[file_list->open_file_n-1] ;  ){
            *i = *(++i);
        }
        file_list_increment_active_pointer(file_list);
        file_list->open_file_n--;
    }
     
}

void file_list_save_and_close_file(file_list_t* file_list){
    save_file(file_list->active_file);
    file_list_force_close_file(file_list);
}


void file_list_close_all(file_list_t* file_list){
    for (int i = 0; i < file_list->open_file_n;i++){
        close_file(&file_list->open_files[i]);
    }
}