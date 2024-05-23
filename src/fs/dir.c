#include "aloe/fs.h"


path_lookup_result_t lookup_path(const char *path){
    struct stat statbuf;
    if (stat(path, &statbuf) != 0){
        return 0;
    }
    if (S_ISREG(statbuf.st_mode)){
        return valid_file;
    }else if (S_ISDIR(statbuf.st_mode)){
        return valid_directory;
    }
    return invalid;
}