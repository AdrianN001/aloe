#ifndef __DIR_H_
#define __DIR_H_

#include <sys/stat.h>


typedef enum path_res{
    valid_file,
    valid_directory,
    invalid,
} path_lookup_result_t;

path_lookup_result_t lookup_path(const char *path);



#endif