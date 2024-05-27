
#include "aloe/buffer.h"
#include "aloe/assert.h"
#include <string.h>
#include <stdlib.h>



buffer_t buffer_init_by_array(char* input, size_t size_of_inputbuffer, const int additional_storage){
    return (buffer_t){
        .data=input,
        .pointer=size_of_inputbuffer,
        .max_size=size_of_inputbuffer+additional_storage
    };

}

buffer_t buffer_init(int size, int max_size){
    const int additional_storage_space = 30;
    char* data = calloc(sizeof(char), additional_storage_space);
    data[additional_storage_space] = '\0';
    return (buffer_t){
        .data=data,
        .pointer=0,
        .max_size=additional_storage_space,
    };
}

void buffer_free(buffer_t* buffer){
    free(buffer->data);
}
void buffer_resize(buffer_t* buffer, size_t new_buffer_size){
    if (new_buffer_size < buffer->max_size){
        return;
    }
    buffer->data = realloc(buffer->data, new_buffer_size * sizeof(char));
    assert_with_log_s(buffer->data != NULL, "Reallocation failed");
    // buffer->max_size = new_buffer_size;
}


void buffer_write_at(buffer_t* buffer, char charater, int position){
    if (buffer->pointer == buffer->max_size){
        buffer_resize(buffer, 2*buffer->max_size);
    }
    /* Append to the end of the file */
    if (position == buffer->pointer){
        buffer->data[position] = charater; 
        buffer->pointer++;
        buffer->data[buffer->pointer] = '\0';
        return;
    }
    /* Push the buffer */
    buffer->pointer++;
    for (int i = buffer->pointer; i >= position; i--){
        buffer->data[i] = buffer->data[i-1];
    }
    buffer->data[position] = charater;

}

void buffer_delete_at(buffer_t* buffer, int position){
    if (buffer->pointer == 0){
        return;
    }
    /* End of the file */
    if (position == buffer->pointer){
        buffer->data[buffer->pointer--] = '\0';
        return;
    }
    for (int i = position; i < buffer->pointer;i++){
        buffer->data[i] = buffer->data[i+1];
    }
    buffer->data[buffer->pointer--] = '\0';

}