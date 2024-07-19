#include <sys/inotify.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "aloe/file_monitor.h"
#include "aloe/assert.h"


int inotify_file_descriptor;

void initilialize_inotify_instance(void){
    inotify_file_descriptor = inotify_init();
}

void deinitilialize_inotify_instance(void){
    close(inotify_file_descriptor);
}

file_monitor_instance_t create_file_monitor_instance(char* path, int flags){
    int watch_descriptor = inotify_add_watch(inotify_file_descriptor, path, flags);
    assert(watch_descriptor > 0);

    return (file_monitor_instance_t){
        .flags = flags,
        .watch_descriptor = watch_descriptor
    };
}





int setup_non_blocking_mode(int inotify_file_descriptor){
    int file_flags = fcntl(inotify_file_descriptor, F_GETFL, 0);
    if (file_flags == -1) {
        return -1;
    }
    if (fcntl(inotify_file_descriptor, F_SETFL, file_flags | O_NONBLOCK) == -1) {
        return -1;
    }

    return 0;
}






void free_file_monitor_instance(file_monitor_instance_t* instance){
    inotify_rm_watch(inotify_file_descriptor, instance->watch_descriptor);
}