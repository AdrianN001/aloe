#include <string.h>
#include <stdlib.h>

#include "aloe/buffer.h"

complex_buffer_t complex_buffer_init(size_t size_of_buffer){
    return (complex_buffer_t){
        .data = malloc(sizeof(buffer_t) * size_of_buffer),
        .max_size = size_of_buffer,
        .pointer = 0
    };
}

void complex_buffer_append(complex_buffer_t* buffer, char* input, size_t size_of_input){
    
    char* input_copy = malloc(sizeof(char) * size_of_input+50);
    memcpy(input_copy, input, size_of_input);
    input_copy[size_of_input] = '\0';

    
    buffer->data[buffer->pointer++] = (buffer_t){
        .data=input_copy,
        .pointer = size_of_input,
        .max_size = size_of_input + 50
    };
}


void complex_buffer_free(complex_buffer_t* buffer){
    for (int i = 0; i < buffer->pointer; i++){
        buffer_free(&buffer->data[i]);
    }
    free(buffer->data);
}