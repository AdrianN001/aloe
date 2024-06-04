#include "aloe/fs/features.h"
#include "aloe/buffer.h"


file_length_data_t get_file_length(file_t* file){
    int number_of_chars = 0;
    int number_of_rows = file->buffer.pointer;


    for (int i = 0; i < file->buffer.pointer; i++){
        buffer_t* row_buffer = &file->buffer.data[i];
        number_of_chars += row_buffer->pointer;
    }


    return (file_length_data_t){
        .number_of_characters = number_of_chars,
        .number_of_rows = number_of_rows,
        .number_of_words = 0
    };
}
