#include <fs.h>
#include "ramdisk.h"
#include "device.h"

/* Files number */
#define FS_NEM  85

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

enum {FD_STDIN, FD_STDOUT, FD_STDERR, DEV_EVENT, PROC_DISPINFO, FD_FB};

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
  [FD_STDOUT] = {"stdout", 0, 0, 0,invalid_read, serial_write},
  [FD_STDERR] = {"stderr", 0, 0, 0,invalid_read, serial_write},
  [DEV_EVENT] = {"/dev/events",0, 0, 0,events_read, invalid_write},
  [PROC_DISPINFO] = {"/proc/dispinfo",0, 0, 0,dispinfo_read, invalid_write},
  [FD_FB] = {"/dev/fb", 0, 0, 0,invalid_read, fb_write},
#include "files.h"
};

void init_fs() {
  // TODO: initialize the size of /dev/fb
  AM_GPU_CONFIG_T VGA = io_read(AM_GPU_CONFIG);
  int w = VGA.width;
  int h = VGA.height;
  file_table[FD_FB].size = w*h*sizeof(uint32_t);
}


/*open the file */
int fs_open(const char *pathname){
  for(int i = 0;i < FS_NEM; i++){
    if(strcmp(file_table[i].name, pathname) == 0){
      //Log("open success %s",file_table[i].name);
      if(i < FD_FB){
        return i;
      }
      file_table[i].open_offset = 0;
      return i;
    }
  }
  panic("Can not find %s ", pathname);
}


/*close the file*/
int fs_close(int fd){
 //Log("close success");
  return 0;
}

/* read file */
size_t fs_read(int fd, void *buf, size_t len){
  ReadFn real_read = file_table[fd].read;
  if(real_read != NULL){
    return real_read(buf,0,len);
  }
  size_t read_len = len;
  size_t open_offset = file_table[fd].open_offset;
  size_t size = file_table[fd].size;
  size_t disk_offset = file_table[fd].disk_offset;
  if (open_offset > size) return 0;
  if (open_offset + len > size) read_len = size - open_offset;
  ramdisk_read(buf, disk_offset + open_offset, read_len);
  file_table[fd].open_offset += read_len;
  return read_len;
}



/* write file */
size_t fs_write(int fd, void *buf, size_t len){
  WriteFn real_write = file_table[fd].write;
  if(real_write != NULL){
    return real_write(buf,0,len);
  }
  size_t write_len = len;
  size_t open_offset = file_table[fd].open_offset;
  size_t size = file_table[fd].size;
  size_t disk_offset = file_table[fd].disk_offset;
  if (open_offset > size) return 0;
  if (open_offset + len > size) write_len = size - open_offset;
  ramdisk_write(buf, disk_offset + open_offset, write_len);
  file_table[fd].open_offset += write_len;
  return write_len;
}


/* locate the file*/
size_t fs_lseek(int fd,size_t offset, int whence){
  if (fd <= 2) {
        Log("ignore lseek %s", file_table[fd].name);
        return 0;
  }
 
  Finfo *file = &file_table[fd];
  size_t new_offset;
  // 根据 whence 参数来计算新的指针位置
    if (whence == SEEK_SET) {
        new_offset = offset;
    } else if (whence == SEEK_CUR) {
        new_offset = file->open_offset + offset;
    } else if (whence == SEEK_END) {
        new_offset = file->size + offset;
    } else {
        Log("Invalid whence value: %d", whence);
        return -1;
    }
     // 检查新的指针位置是否在文件范围内
    if (new_offset < 0 || new_offset > file->size) {
        Log("Seek position out of bounds");
        return -1;
    }
     // 设置新的文件读写指针
    file->open_offset = new_offset;
    
    return new_offset;
}

