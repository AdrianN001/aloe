#ifndef __FILE_SEARCH_H_
#define __FILE_SEARCH_H_

#include "dir.h"

#define MAX_FILE_SEARCH_RESULT 15

typedef struct {

    char** result;
    size_t n_of_results;
} file_search_result_t;

file_search_result_t search_for_file(dir_t* directory, char* file_rgx);
void free_file_search_result(file_search_result_t* res);

#endif
