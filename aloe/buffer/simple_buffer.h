#ifndef __SIMPLE_BUFFER_H_
#define __SIMPLE_BUFFER_H_

#include <stddef.h>

typedef struct buffer{
    char* data;
    int pointer;
    int max_size;

} buffer_t;

/* One way to construct this buffer is taking in a previously buffer from a file opening, and having its ownership */
buffer_t buffer_init_by_array(char* input, size_t size_of_inputbuffer, const int additional_storage);

/* The other way is to create a fresh, new buffer */
buffer_t buffer_init(int size, int max_size);

void buffer_free(buffer_t* buffer);
void buffer_resize(buffer_t* buffer, size_t new_buffer_size);


void buffer_write_at(buffer_t* buffer, char charater, int position);
void buffer_delete_at(buffer_t* buffer, int position);



#endif