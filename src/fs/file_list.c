
#include "aloe/fs.h"
#include <string.h>
#include <stdlib.h>
#include "aloe/assert.h"


file_list_t file_list_init(int max_files_n){
    file_t* data = malloc(sizeof(file_t) * MAX_FILES_ALLOWED);
    return (file_list_t){
        .active_file = NULL,
        .open_files = data,
        .open_file_n = 0
    };

}

void file_list_free(file_list_t* file_list){
    free(file_list->open_files);
    file_list->active_file = NULL;
    file_list->open_file_n = 0;
}

void file_list_switch_to(file_list_t* file_list, int index){
    if (index <= file_list->open_file_n){
        return;
    }
    file_list->active_file = &(file_list->open_files[index]);
}


int file_list_try_close_file(file_list_t* file_list){
    if (file_list->active_file->dirty){
        return -1;
    }
    file_list_force_close_file(file_list);
}

void file_list_force_close_file(file_list_t* file_list){
    assert_with_log_s(0, "Not yet implemented");
}

void file_list_save_and_close_file(file_list_t* file_list){
    save_file(&(file_list->active_file));
    file_list_force_close_file(file_list);
}