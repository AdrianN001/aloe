#ifndef __FILE_MONITOR_H_
#define __FILE_MONITOR_H_

#include <sys/inotify.h>

#define FILE_INOTIFY_FLAGS      (IN_MODIFY | IN_MOVE_SELF )
#define DIR_INOTIFY_FLAGS       (IN_CREATE | IN_DELETE )

/* Events */
#define NO_EVENT -1



#define EVENT_BUFFER_LENGTH (32 * (sizeof(struct inotify_event)))

typedef struct {
    int watch_descriptor;
    int flags;
} file_monitor_instance_t;

extern int inotify_file_descriptor;

void initilialize_inotify_instance(void);
void deinitilialize_inotify_instance(void);


file_monitor_instance_t create_file_monitor_instance(char* path, int flags);


int setup_non_blocking_mode(int inotify_file_descriptor);


void free_file_monitor_instance(file_monitor_instance_t* instance);

#endif