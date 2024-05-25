#ifndef __FILE_INFO_H_
#define __FILE_INFO_H_

#include "file.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>

typedef struct {

    char* file_type;
    char* file_name;
    
    char* file_size;
    char* file_owner;

    char* last_time_accessed;
    char* last_time_modified; 

} file_info_t;

char* get_file_extension_from_path(char* file_path);
const char* match_file_extension_with_file_type(const char* file_name);

char* format_file_size(long long bytes, char* out);

char* get_owner(struct stat* info);

file_info_t get_file_metadata(file_t* file);



#endif