#include "aloe/fs.h"
#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


int lookup_path(const char *path){
    struct stat statbuf;
    if (stat(path, &statbuf) != 0){
        return INVALID;
    }
    if (S_ISREG(statbuf.st_mode)){
        return VALID_FILE;
    }else if (S_ISDIR(statbuf.st_mode)){
        return VALID_DIRECTORY;
    }
    return INVALID;
}





dir_t create_directory_object(const char* path,  int max_depth){
 
    char** files = malloc(sizeof(char*) * MAX_FILES);
    dir_t* subdirs = malloc(sizeof(dir_t) * MAX_SUBDIR);
    
    size_t length_of_path = strlen(path);
    char* path_copy = malloc(sizeof(char) * length_of_path+1);
    strcpy(path_copy,path);
    path_copy[length_of_path] = '\0';


    int files_p = 0;
    int subdirs_p = 0;
    
    struct dirent *dir_entry;
    DIR* directory = opendir(path);
    if (!directory){
        return (dir_t){};
    }
    while ((dir_entry = readdir(directory)) != NULL) {
        if(strcmp(dir_entry->d_name, ".") == 0 || strcmp(dir_entry->d_name, "..") == 0 || strcmp(dir_entry->d_name, ".git") == 0 ){
            continue;
        }
        if (dir_entry->d_type == DT_DIR && max_depth > 0){

            char* new_path = malloc(sizeof(char)*128);
            strcpy(new_path, path);
            new_path[strlen(path)] = '/';
            strcpy(&new_path[strlen(path)+1], dir_entry->d_name);
            
            if (subdirs_p < MAX_SUBDIR){
                subdirs[subdirs_p++] = create_directory_object(new_path, max_depth-1);
            }
            }else{
                if (files_p < MAX_FILES){
                    size_t len_of_file_name = strlen(dir_entry->d_name);
                    files[files_p] = malloc(sizeof(char) * len_of_file_name+1);
                    strcpy(files[files_p], dir_entry->d_name);
                    files[files_p][len_of_file_name] = '\0';
                    files_p++;
                }
            }
    }
    closedir(directory);

    return (dir_t){
        .dir_path = path_copy,

        .files = files,
        .n_files = files_p,
        
        .subdirectories = subdirs,
        .n_subdir = subdirs_p,

        .pointer = 0
    };

}

void  free_directory_object(dir_t* dir_obj){
    for (int i = 0; i < dir_obj->n_files;i++){
        free(dir_obj->files[i]);
    }
    free(dir_obj->files);

    for (int i = 0; i < dir_obj->n_subdir;i++){
        free_directory_object(&dir_obj->subdirectories[i]);
    }
    free(dir_obj->subdirectories);
    free(dir_obj->dir_path);
}


int make_new_directory(char* path){
    struct stat st = {0};

    if(stat(path, &st) == -1){
        mkdir(path, 0700);
        return 1;
    }
    return -1;
}
