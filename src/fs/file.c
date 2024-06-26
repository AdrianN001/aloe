
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "aloe/fs.h"
#include "aloe/assert.h"
#include "aloe/buffer.h"
#include "aloe/file_monitor.h"

char* get_filename_by_path(char* path){
    int last_slash_character = 0;
    bool slash_found = false;
    char* path_cpy = path;
    int offset = 0;
    for (;(*path_cpy != 0); path_cpy++){
        if (*path_cpy == '/'){
            last_slash_character = offset;
            slash_found = true;
        }
        offset++;
    }
    return slash_found ? &(path[last_slash_character+1]) : &(path[last_slash_character]) ;
}


file_t open_file(char* path){
    const int max_lines = 100;

    FILE *fp = fopen(path, "r");
   
    char* line = NULL;
    int chars_read;
    size_t len = 0;
    bool any_line_read = false;

    complex_buffer_t lines_buffer = complex_buffer_init(max_lines);
    
    while((chars_read = getdelim(&line, &len, '\n', fp)) != -1){
        if (chars_read == 0){
            complex_buffer_append_blank(&lines_buffer);
        }else{
            complex_buffer_append(&lines_buffer, line, chars_read);
        }
        any_line_read = true;
    }
    if (!any_line_read){
        complex_buffer_append_blank(&lines_buffer);
    }
    fseek(fp, 0L, SEEK_SET);

    size_t len_of_path = strlen(path);
    char* path_copy = malloc(sizeof(char) * len_of_path+1);
    strncpy(path_copy, path, len_of_path);
    path_copy[len_of_path] = '\0';

    file_monitor_instance_t file_monitor = create_file_monitor_instance(path_copy, FILE_INOTIFY_FLAGS);

    
    return (file_t){
        .buffer = lines_buffer,
        .dirty = false,
        .file_name = get_filename_by_path(path_copy),
        .absolute_file_name = path_copy,
        .fp = fp,
        .row_pointer = 0,
        .row_offset = 0,
        .collumn_pointer = 0,
        .collumn_offset = 0,
        .file_monitor = file_monitor
    };

}

file_t create_new_file(char* file_name){
    const int max_lines = 100;
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL){
        return (file_t){.fp=NULL};
    }

    complex_buffer_t lines_buffer = complex_buffer_init(max_lines);
    complex_buffer_append_blank(&lines_buffer);


    size_t len_of_path = strlen(file_name);
    char* path_copy = malloc(sizeof(char) * len_of_path+1);
    strncpy(path_copy, file_name, len_of_path);
    path_copy[len_of_path] = '\0';


    //TODO The file monitor assertion fails
    file_monitor_instance_t file_monitor = create_file_monitor_instance(path_copy, FILE_INOTIFY_FLAGS);


    return (file_t){
        .buffer = lines_buffer,
        .dirty = false,
        .file_name = get_filename_by_path(path_copy),
        .absolute_file_name = path_copy,
        .fp = fp,
        .row_pointer = 0,
        .row_offset = 0,
        .collumn_pointer = 0,
        .collumn_offset = 0,
        .file_monitor = file_monitor
    };
}



int write_to_file(file_t* file, char character){

    buffer_t* row_buffer = &(file->buffer.data[file->row_pointer]);

    buffer_write_at(row_buffer, character, file->collumn_pointer);

    return 0;
}
int delete_from_file(file_t* file){
    buffer_t* row_buffer = &(file->buffer.data[file->row_pointer]);

    buffer_delete_at(row_buffer, file->collumn_pointer);

    return 0;
}

int insert_new_line_to_file(file_t* file){
    file->dirty = true;
    buffer_t* prev_line = &file->buffer.data[file->row_pointer];
    int coll_p = file->collumn_pointer;
    
    /*EOL*/
    if( coll_p == prev_line->pointer){
        complex_buffer_insert_at(&file->buffer, file->row_pointer + 1);
        file->collumn_pointer = 0;
        file->row_pointer++;
        return 0;
    }

    char* substring_on_newline = &(prev_line->data[coll_p]);

    complex_buffer_insert_at(&file->buffer, file->row_pointer + 1);
    file->row_pointer++;
    file->collumn_pointer = 0;
    
    buffer_t* new_line = &file->buffer.data[file->row_pointer];
    
    char chr;
    while((chr = *substring_on_newline++) != '\0'){
        buffer_write_at(new_line, chr, new_line->pointer);
    }

    prev_line->data[coll_p] = '\0';
    prev_line->pointer = coll_p;

    return 0;
    
}

int save_file(file_t* file){
    file->fp = freopen(NULL, "w", file->fp);
    assert(file->fp != NULL);

    complex_buffer_t* buffer = &file->buffer;


    for (int i = 0; i < buffer->pointer;i++){
        buffer_t* current_line = &buffer->data[i];
        if (current_line->pointer==0){
            fprintf(file->fp, "\n");
        }else if(i == buffer->pointer -1){
            fprintf(file->fp, "%s", current_line->data);

        }else{
            fprintf(file->fp, "%s\n", current_line->data);
        }
        
    }
    fflush(file->fp);
    file->fp = freopen(NULL, "r", file->fp);
    assert(file->fp != NULL);
    return 0;
}


int close_file(file_t* file){
    free(file->absolute_file_name);
    complex_buffer_free(&(file->buffer));
    fclose(file->fp);
    free_file_monitor_instance(&file->file_monitor);
    return 0;
}



int refresh_file_buffer(file_t* file){
    complex_buffer_free(&file->buffer);


    char* line = NULL;
    int chars_read;
    size_t len = 0;
    bool any_line_read = false;

    complex_buffer_t lines_buffer = complex_buffer_init(100);
    
    while((chars_read = getdelim(&line, &len, '\n', file->fp)) != -1){
        if (chars_read == 0){
            complex_buffer_append_blank(&lines_buffer);
        }else{
            complex_buffer_append(&lines_buffer, line, chars_read);
        }
        any_line_read = true;
    }
    if (!any_line_read){
        complex_buffer_append_blank(&lines_buffer);
    }

    file->buffer = lines_buffer;
    return 0;

}
