#include "aloe/fs.h"
#include <regex.h>
#include <stdlib.h>

file_search_result_t search_for_file(dir_t* directory, char* file_rgx){
    regex_t regex_obj;
    (void)regcomp(&regex_obj, file_rgx, 0);

    char** result = malloc(sizeof(char*) * MAX_FILE_SEARCH_RESULT);
    size_t p = 0;

    for (int i = 0; i < directory->n_files; i++){
        if(regexec(&regex_obj, directory->files[i], 0, NULL, 0) == 0){
            result[p] = malloc(sizeof(char) * 256);
            snprintf(result[p], 256, "%s", directory->files[i] );

            p++;
        }
    }
    
    for (int i = 0; i < directory->n_subdir && p < MAX_FILE_SEARCH_RESULT; i++){
        file_search_result_t subdir_results = search_for_file(&directory->subdirectories[i], file_rgx);
        
        for(int j = 0; p < MAX_FILE_SEARCH_RESULT && j < subdir_results.n_of_results ; p++, j++){
            result[p] = malloc(sizeof(char) * 256);
            snprintf(result[p], 256, "%s/%s", directory->subdirectories[i].dir_path, subdir_results.result[j]);
        }

        free_file_search_result(&subdir_results);
    }

    return (file_search_result_t){
        .result = result,
        .n_of_results = p
        };
}

void free_file_search_result(file_search_result_t* res){
    for (int i = 0; i< res->n_of_results;i++){
        free(res->result[i]);
    }
    free(res->result);
}