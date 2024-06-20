#include <fs.h>
#include "ramdisk.h"

/* Files number */
#define FS_NEM  24

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, 0,invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, 0,invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, 0,invalid_read, invalid_write},
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
}


/*open the file */

int fs_open(const char *pathname){
  for(int i = 3;i < FS_NEM; i++){
    if(strcmp(file_table[i].name, pathname) == 0){
      Log("open success");
      file_table[i].open_offset = 0;
      return i;
    }
  }
  panic("Can not find %s ", pathname);
}


/*close the file*/
int fs_close(int fd){
  Log("close success");
  return 0;
}

/* read file */
size_t fs_read(int fd, void *buf, size_t len){
  size_t disk_offset = file_table[fd].disk_offset;
  size_t open_offset = file_table[fd].open_offset;
  ramdisk_read(buf,disk_offset + open_offset,len);
  file_table[fd].open_offset += len;
  return len;
}



/* write file */
size_t fs_write(int fd, void *buf, size_t len){
  size_t disk_offset = file_table[fd].disk_offset;
  size_t open_offset = file_table[fd].open_offset;
  ramdisk_write(buf,disk_offset + open_offset,len);
  file_table[fd].open_offset += len;
  return len; 
}


/* locate the file*/
size_t fs_lseek(int fd,size_t offset, int whence){
  size_t new_offset = 0;
  switch(whence){
    case SEEK_SET:
      new_offset = offset;
      break;
    case SEEK_CUR:
      new_offset = offset + file_table[fd].open_offset;
      break;
    case SEEK_END:
      new_offset = file_table[fd].size + offset; 
      break;
  }
  file_table[fd].open_offset = new_offset;
  return new_offset;
}

