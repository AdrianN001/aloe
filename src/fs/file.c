#include "aloe/fs.h"
#include "aloe/assert.h"
#include "aloe/buffer.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



file_t open_file(char* path){
    const int additional_storage_in_buffer = 50;
    const int max_lines = 100;

    FILE *fp = fopen(path, "r");
    assert(fp != NULL);
    
    /* Go to the end of the file. */
    assert(fseek(fp, 0L, SEEK_END) == 0);
    /* Get the size of the file. */
    long bufsize = ftell(fp);    
    assert(bufsize != 0);

    char* content = (char*)malloc(sizeof(char) * (bufsize + 1));
    
    /* Go back to the start of the file. */
    assert(fseek(fp, 0L, SEEK_SET) == 0)

    /* Read the entire file into memory. */
    size_t newLen = fread(content, sizeof(char), bufsize, fp);
    content[bufsize] = '\0';

    //buffer_t new_buffer = buffer_init_by_array(content, bufsize+additional_storage_in_buffer, additional_storage_in_buffer);
    complex_buffer_t lines_buffer = complex_buffer_init(max_lines);
    const char delimiter[] = "\n";

    char* line = strtok(content, delimiter);

    while( line != NULL ){
        complex_buffer_append(&lines_buffer, line, strlen(line));
        line = strtok(NULL, delimiter);
    }
    
    free(content);

    return (file_t){
        .buffer = lines_buffer,
        .dirty = false,
        .file_name = "%path%",
        .fp = fp,
        .row_pointer = 0,
        .row_offset = 0,
        .collumn_pointer = 0
    };

}

int write_to_file(file_t* file){
    assert_with_log_s(0, "Not yet implemented");
    return -1;
}
int delete_from_file(file_t* file){
    assert_with_log_s(0, "Not yet implemented");
    return -1;
}
int save_file(file_t* file){
    assert_with_log_s(0, "Not yet implemented");
    return -1;
}


int close_file(file_t* file){
    complex_buffer_free(&(file->buffer));
    return fclose(file->fp);
}