#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdbool.h>
#include "aloe/buffer.h"
#include "aloe/assert.h"

#define ADDITIONAL_STORAGE_ALLOCATED 30

complex_buffer_t complex_buffer_init(size_t size_of_buffer){
    return (complex_buffer_t){
        .data = calloc(sizeof(buffer_t), size_of_buffer),
        .max_size = size_of_buffer,
        .pointer = 0
    };
}

void complex_buffer_append_blank(complex_buffer_t* buffer){
    if (buffer->pointer+1 == buffer->max_size){
        buffer->max_size *= 2;
        complex_buffer_resize(buffer, buffer->max_size);
    }
    char* blank_line = calloc(sizeof(char), ADDITIONAL_STORAGE_ALLOCATED);

    buffer->data[buffer->pointer++] = buffer_init_by_array(blank_line, 0, ADDITIONAL_STORAGE_ALLOCATED);

}

void complex_buffer_append(complex_buffer_t* buffer, char* input, size_t size_of_input){
    if (buffer->pointer+1 == buffer->max_size){
        buffer->max_size *= 2;
        complex_buffer_resize(buffer, buffer->max_size);
    }
    size_t size_of_copied_buffer = (size_of_input+ADDITIONAL_STORAGE_ALLOCATED+1);
    char* input_copy = calloc(sizeof(char), size_of_copied_buffer);
    assert(input_copy != NULL);

    bool endl_removed = false;
    for (int i = 0; i < size_of_input; i++){
        if ((int)input[i] == 10){
            input_copy[i] = '\0';
            endl_removed = true;
            continue;
        }
        input_copy[i] = input[i];
    }

    buffer->data[buffer->pointer++] = buffer_init_by_array(input_copy, size_of_input - (int)endl_removed, ADDITIONAL_STORAGE_ALLOCATED);
  
}

void complex_buffer_insert_at(complex_buffer_t* buffer, int pointer){
    char* data_of_new_line = calloc(sizeof(char), ADDITIONAL_STORAGE_ALLOCATED);
    if (pointer == buffer->pointer){
        buffer->data[buffer->pointer++] = (buffer_t){
            .data=data_of_new_line,
            .max_size= ADDITIONAL_STORAGE_ALLOCATED,
            .pointer=0
        };
        return;
    }
    buffer->pointer++;
    for (int i = buffer->pointer; i >= pointer; i--){
        buffer->data[i] = buffer->data[i-1];
    }
    /* Overwrite the data */
    buffer->data[pointer] = (buffer_t){
        .data=data_of_new_line,
        .max_size=ADDITIONAL_STORAGE_ALLOCATED,
        .pointer=0
    };
    if (buffer->pointer == buffer->max_size){
        complex_buffer_resize(buffer, buffer->max_size*2);
        buffer->max_size *= 2;
    }

}

void complex_buffer_remove_from(complex_buffer_t* buffer, int pointer){
    /*Merges into the previous lane*/
    buffer_t* prev_lane_buffer = &(buffer->data[pointer-1]);
    buffer_t* current_lane_buff = &(buffer->data[pointer]);

    for(int i = 0; i < current_lane_buff->pointer; i++){
        buffer_write_at(prev_lane_buffer, current_lane_buff->data[i], prev_lane_buffer->pointer);
    }
    buffer_free(current_lane_buff);

    for (int i = pointer; i < buffer->pointer; i++){
        buffer->data[i] = buffer->data[i+1];
    }
    buffer->pointer--;
}

void complex_buffer_resize(complex_buffer_t* buffer, size_t new_size){

    buffer->data  = (buffer_t*)realloc(buffer->data, new_size*sizeof(buffer_t));

    assert_with_log_s(buffer->data != NULL, "Reallocation failed");
}

void complex_buffer_free(complex_buffer_t* buffer){
    for (int i = 0; i < buffer->pointer; i++){
        buffer_free(&buffer->data[i]);
    }
    free(buffer->data);
}