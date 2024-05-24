#include <string.h>
#include <stdlib.h>

#include "aloe/buffer.h"

#define ADDITIONAL_STORAGE_ALLOCATED 50

complex_buffer_t complex_buffer_init(size_t size_of_buffer){
    return (complex_buffer_t){
        .data = malloc(sizeof(buffer_t) * size_of_buffer),
        .max_size = size_of_buffer,
        .pointer = 0
    };
}

void complex_buffer_append(complex_buffer_t* buffer, char* input, size_t size_of_input){
    
    char* input_copy = malloc(sizeof(char) * size_of_input+ADDITIONAL_STORAGE_ALLOCATED);
    memcpy(input_copy, input, size_of_input);
    input_copy[size_of_input] = '\0';

    
    buffer->data[buffer->pointer++] = (buffer_t){
        .data=input_copy,
        .pointer = size_of_input,
        .max_size = size_of_input + ADDITIONAL_STORAGE_ALLOCATED
    };
}

void complex_buffer_insert_at(complex_buffer_t* buffer, int pointer){
    char* data_of_new_line = malloc(sizeof(char) * ADDITIONAL_STORAGE_ALLOCATED);
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



void complex_buffer_free(complex_buffer_t* buffer){
    for (int i = 0; i < buffer->pointer; i++){
        buffer_free(&buffer->data[i]);
    }
    free(buffer->data);
}