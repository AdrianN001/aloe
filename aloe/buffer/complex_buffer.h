#ifndef __COMPLEX_BUFFER_H_
#define __COMPLEX_BUFFER_H_

#include <stddef.h>
#include "simple_buffer.h"

typedef struct complex_buff{
    buffer_t* data;
    size_t pointer;
    size_t max_size;
} complex_buffer_t;


complex_buffer_t complex_buffer_init(size_t size_of_buffer);

void complex_buffer_append(complex_buffer_t* buffer, char* input, size_t size_of_input);
void complex_buffer_insert_at(complex_buffer_t* buffer, int pointer);
void complex_buffer_remove_from(complex_buffer_t* buffer, int pointer);


void complex_buffer_free(complex_buffer_t* buffer);



#endif