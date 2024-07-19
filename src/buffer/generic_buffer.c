#include "aloe/buffer.h"
#include "aloe/assert.h"
#include "aloe/terminal.h"
#include <stdlib.h>
#include <string.h>


generic_buffer_t generic_buffer_init(size_t sizeof_data, size_t array_size){
    void* data = malloc(array_size* sizeof_data);

    return (generic_buffer_t){
        .data = data,
        .max_size = array_size,
        .size_of_data = sizeof_data,
        .pointer = 0
    };

}

void generic_buffer_append(generic_buffer_t* buffer, void* data){
    if (buffer->pointer + buffer->size_of_data == buffer->max_size ){return;}
    memmove(&buffer->data[buffer->pointer], data, buffer->size_of_data);
    buffer->pointer += buffer->size_of_data;
}

void* generic_buffer_at(generic_buffer_t* buffer, int pointer){
    return &buffer->data[pointer* buffer->size_of_data];
}

void generic_buffer_delete_one(generic_buffer_t* buffer){
    if(buffer->pointer == 0){return;}
    buffer->pointer--;
}

void generic_buffer_free(generic_buffer_t* buffer){
    free(buffer->data);
}