
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#include "aloe/file_monitor.h"
#include "aloe/assert.h"
#include "aloe/fs.h"


#ifndef MAX
#define MAX(x, y) ((x < y) ? y : x)
#endif

file_list_t file_list_init(){
    file_t* data = calloc(sizeof(file_t), MAX_FILES_ALLOWED);
    return (file_list_t){
        .active_file = NULL,
        .open_files = data,
        .active_file_pointer = 0,
        .open_file_n = 0
    };
}

void file_list_free(file_list_t* file_list){
    free(file_list->open_files);
    file_list->active_file = NULL;
    file_list->open_file_n = 0;
}

file_t* file_list_append(file_list_t* file_list, char* path){
    if( file_list->open_file_n >= MAX_FILES_ALLOWED){
        return NULL;
    }
    // Check if the file was opened previously
    for ( int i = 0; i < file_list->open_file_n; i++){
        if (strcmp(path, file_list->open_files[i].absolute_file_name) == 0){
            return NULL;
        }
    }

    file_list->open_files[file_list->open_file_n] = open_file(path);
    if (file_list->active_file == NULL ){
        file_list->active_file = &file_list->open_files[file_list->open_file_n];
    }
    return &file_list->open_files[file_list->open_file_n++];
}

file_t* file_list_append_premade(file_list_t* file_list, file_t file){
    if( file_list->open_file_n >= MAX_FILES_ALLOWED){
        return NULL;
    }

    file_list->open_files[file_list->open_file_n] = file;
    if (file_list->active_file == NULL ){
        file_list->active_file = &file_list->open_files[file_list->open_file_n];
    }
    return &file_list->open_files[file_list->open_file_n++];
}

void file_list_increment_active_pointer(file_list_t* file_list){

    file_list->active_file_pointer = (file_list->active_file_pointer + 1) % file_list->open_file_n;
    file_list->active_file = &file_list->open_files[file_list->active_file_pointer];
}

void file_list_decrement_active_pointer(file_list_t* file_list){

    file_list->active_file_pointer = (file_list->active_file_pointer == 0) ? file_list->open_file_n -1: file_list->active_file_pointer - 1;

    file_list->active_file = &file_list->open_files[file_list->active_file_pointer];
}



int file_list_try_close_file(file_list_t* file_list){
    if (file_list->active_file->dirty){
        return -1;
    }
    file_list_force_close_file(file_list);
    return 0;
}

void file_list_force_close_file(file_list_t* file_list){
    close_file(file_list->active_file);
    if(file_list->active_file == &file_list->open_files[file_list->open_file_n-1]){
        file_list->open_file_n = MAX(file_list->open_file_n -1, 0) ;
        file_list->active_file_pointer = MAX(file_list->active_file_pointer -1, 0);
        file_list->active_file = (file_list->open_file_n == 0 )? NULL : &file_list->open_files[file_list->active_file_pointer];
    }
    else{
        /* Pointer arithmetic <3 */
        for (file_t* i = file_list->active_file; i != &file_list->open_files[file_list->open_file_n-1] ;  ){
            *i = *(++i);
        }
        file_list_increment_active_pointer(file_list);
        file_list->open_file_n--;
    }
     
}

void file_list_save_and_close_file(file_list_t* file_list){
    save_file(file_list->active_file);
    file_list_force_close_file(file_list);
}


void file_list_close_all(file_list_t* file_list){
    for (int i = 0; i < file_list->open_file_n;i++){
        close_file(&file_list->open_files[i]);
    }
}


void file_list_handle_file_events(file_list_t* file_list){
    char buffer[EVENT_BUFFER_LENGTH] = {0};
    int read_length = read(inotify_file_descriptor, buffer, EVENT_BUFFER_LENGTH);

    if (read_length < 0 && errno == EAGAIN){
        return ;
    }else if (read_length < 0){
        assert(0);
    }

    int index = 0;

    while (index < read_length){
        struct inotify_event* event = (struct inotify_event* )&buffer[index];
        
        file_t* file_event_source = get_file_from_watch_descriptor(file_list, event->wd);
        // TODO Implement a way to react to file changes

        if(event->mask & IN_MOVE_SELF){
            assert_with_log_s(0, file_event_source->file_name);
        }

        index += sizeof(struct inotify_event) + event->len;

        // switch(instance->flags){
            // case FILE_INOTIFY_FLAGS:{

        //         if (event->mask & IN_MODIwFY){
        //             return IN_MODIFY;
        //         } else if (event->mask & IN_DELETE_SELF){
        //             return IN_DELETE_SELF;
        //         }
        //         break;
        //     }
        //     case DIR_INOTIFY_FLAGS:{
                
        //         if (event->mask & IN_CREATE){
        //             return IN_CREATE;
        //         }else if(event->mask & IN_DELETE){
        //             return IN_DELETE;
        //         }
        //         break;
        //     }
        // }
    }
    return ;
}

file_t* get_file_from_watch_descriptor(file_list_t* file_list, int watch_d){
    for (int i = 0; i < file_list->open_file_n;i++){
        if (file_list->open_files[i].file_monitor.watch_descriptor == watch_d){
            return &file_list->open_files[i];
        }
    }
    return NULL;
}