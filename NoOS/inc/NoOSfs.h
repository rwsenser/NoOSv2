//
// NoOSfs.h
//
// "user "interface" to NoOS file system
//

// prefix: noosfs and _noosfs_

#include "common_types.h"
int noosfs_init ();
int noosfs_mount();
int noosfs_create(char filename[]);
int noosfs_open(char filename[]);
int noosfs_read(int32 fd, char buffer[], size_t buffer_size);
int noosfs_write(int32 fd, char buffer[], size_t buffer_size);
int noosfs_close(int32 fd);
int noosfs_list(char prefix[]);
int noosfs_delete(char filename[]); 
