#include "aloe/fs.h"
#include "aloe/assert.h"
#include "aloe/buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char* get_filename_by_path(char* path){
    int last_slash_character = 0;
    char* path_cpy = path;
    int offset = 0;
    for (;(*path_cpy != 0); path_cpy++){
        if (*path_cpy == '/'){
            last_slash_character = offset;
        }
        offset++;
    }
    return &(path[last_slash_character+1]);
}


file_t open_file(char* path){
    const int additional_storage_in_buffer = 50;
    const int max_lines = 100;

    FILE *read_fp = fopen(path, "r");
    assert(read_fp != NULL);
    
    /* Go to the end of the file. */
    assert(fseek(read_fp, 0L, SEEK_END) == 0);
    /* Get the size of the file. */
    long bufsize = ftell(read_fp);    

    char* content = (char*)malloc(sizeof(char) * (bufsize + 1));
    
    /* Go back to the start of the file. */
    assert(fseek(read_fp, 0L, SEEK_SET) == 0)

    /* Read the entire file into memory. */
    size_t newLen = fread(content, sizeof(char), bufsize, read_fp);
    content[bufsize] = '\0';

    //buffer_t new_buffer = buffer_init_by_array(content, bufsize+additional_storage_in_buffer, additional_storage_in_buffer);
    complex_buffer_t lines_buffer = complex_buffer_init(max_lines);
    const char delimiter[] = "\n";

    if (bufsize > 0){
        char* line = strtok(content, delimiter);

        while( line != NULL ){
            complex_buffer_append(&lines_buffer, line, strlen(line));
            line = strtok(NULL, delimiter);
        }
    }else{
        char empty_buffer[1];
        complex_buffer_append(&lines_buffer, empty_buffer, 0);
    }
   
    
    free(content);
    fclose(read_fp);

    FILE* fp = fopen(path, "w");

    return (file_t){
        .buffer = lines_buffer,
        .dirty = false,
        .file_name = get_filename_by_path(path),
        .fp = fp,
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
    complex_buffer_t* buffer = &file->buffer;

    fseek(file->fp, 0L, SEEK_SET);

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

    return 0;
}


int close_file(file_t* file){
    complex_buffer_free(&(file->buffer));
    return fclose(file->fp);
}

