#ifndef __WORD_COUNTER_H_
#define __WORD_COUNTER_H_

#include "aloe/fs.h"

typedef struct{
    int number_of_rows;

    int number_of_words;
    int number_of_characters;
} file_length_data_t;

file_length_data_t get_file_length(file_t* file);


#endif