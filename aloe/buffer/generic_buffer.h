#ifndef _GENERIC_BUFFER_H_
#define _GENERIC_BUFFER_H_

#include <stdlib.h>

typedef struct {

    void* data;
    int pointer;
    size_t size_of_data;
    size_t max_size;
    
} generic_buffer_t;


generic_buffer_t generic_buffer_init(size_t sizeof_data, size_t array_size);
void  generic_buffer_append(generic_buffer_t* buffer, void* data);
void* generic_buffer_at(generic_buffer_t* buffer, int pointer);

void  generic_buffer_delete_one(generic_buffer_t* buffer);

void generic_buffer_free(generic_buffer_t* buffer);


#endif