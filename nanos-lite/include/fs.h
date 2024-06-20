#ifndef __FS_H__
#define __FS_H__

#include <common.h>

#ifndef SEEK_SET
enum {SEEK_SET, SEEK_CUR, SEEK_END};
#endif

/*include use for operate file*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


/*function use fo operate file*/
int fs_open(const char *pathname);
size_t fs_lseek(int fd,size_t offset, int whence);
size_t fs_write(int fd, void *buf, size_t len);
size_t fs_read(int fd, void *buf, size_t len);
int fs_close(int fd);



#endif
