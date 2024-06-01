#ifndef __DIR_H_
#define __DIR_H_

#include <dirent.h>
#include <sys/stat.h>

#define VALID_FILE          0
#define VALID_DIRECTORY     1
#define INVALID             2
#define NO_FILE_WAS_GIVEN   3

#define MAX_SUBDIR          10
#define MAX_FILES           20


int lookup_path(const char *path);

typedef struct directory{

    char* dir_path;

    struct directory* subdirectories;
    char** files;

    int n_subdir;
    int n_files;

    int pointer;
} dir_t;

dir_t create_directory_object(const char* path, int max_depth);
void  free_directory_object(dir_t* dir_obj);



#endif