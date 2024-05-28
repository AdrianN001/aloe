#include "aloe/fs.h"
#include "aloe/assert.h"
#include "aloe/buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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
    const int additional_storage_in_buffer = 50;
    const int max_lines = 100;

    FILE *read_fp = fopen(path, "r");
   
    char* line = NULL;
    int chars_read;
    int len = 0;
    bool any_line_read = false;

    complex_buffer_t lines_buffer = complex_buffer_init(max_lines);
    
    while((chars_read = getdelim(&line, &len, '\n', read_fp)) != -1){
        if (chars_read == 0){
            complex_buffer_append_blank(&lines_buffer);
        }else{
            complex_buffer_append(&lines_buffer, line, chars_read);
        }
        any_line_read = true;
    }
    if (!any_line_read){
        char empty_buffer[1];
        complex_buffer_append(&lines_buffer, empty_buffer, 0);
    }
    fseek(read_fp, 0L, SEEK_SET);
    
    return (file_t){
        .buffer = lines_buffer,
        .dirty = false,
        .file_name = get_filename_by_path(path),
        .absolute_file_name = path,
        .fp = read_fp,
        .row_pointer = 0,
        .row_offset = 0,
        .collumn_pointer = 0
    };

}

int write_to_file(file_t* file, char character){

    buffer_t* row_buffer = &(file->buffer.data[file->row_pointer]);

    buffer_write_at(row_buffer, character, file->collumn_pointer);

    return -1;
}
int delete_from_file(file_t* file){
    buffer_t* row_buffer = &(file->buffer.data[file->row_pointer]);

    buffer_delete_at(row_buffer, file->collumn_pointer);

    return -1;
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
    complex_buffer_free(&(file->buffer));
    fclose(file->fp);
    return 0;
}

