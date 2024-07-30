#include <common.h>
#include "syscall.h"
#include <fs.h>
#include <sys/time.h>
#include <proc.h>


/*===================strace function===================*/
const char* getSystemCallString(int value) {
  switch(value) {
    case SYS_exit:
      return "SYS_exit";
    case SYS_yield:
      return "SYS_yield";
    case SYS_brk:
      return "SYS_brk";
    case SYS_open:
      return "SYS_open";
    case SYS_read:
      return "SYS_read";
    case SYS_write:
      return "SYS_write";
    case SYS_close:
      return "SYS_close";
    case SYS_lseek:
      return "SYS_lseek";
    case SYS_gettimeofday:
      return "SYS_gettimeofday";
    case SYS_execve:
      return "SYS_execve";
    default:
      return "Unknown system call";
  }
}


int sys_yield() {
    yield();
    return 0;
}

int sys_write(int fd, void *buff, size_t count){
  return fs_write(fd, buff, count);
}

void sys_exit(int status) {   
  halt(status);
}


int sys_brk(void *addr){
  return 0;
}

int sys_close(int fd){
  return fs_close(fd);
}

int sys_open(const char *pathname){
  return fs_open(pathname);
}

int sys_read(int fd, void *buff, size_t count){
  return fs_read(fd, buff, count);
}

int sys_lseek(int fd, off_t offset, int whence){
  return fs_lseek(fd, offset, whence);
}

int sys_gettimeofday(struct timeval *tv, struct timezone *tz){
  uint32_t us = io_read(AM_TIMER_UPTIME).us;
  tv->tv_sec = us / 1000000;
  tv->tv_usec = us; 
  return 0;
}

int sys_execve(const char *fname, char *const argv[], char *const envp[]) {
  printf("%s",argv[1]);
  if(strncmp(argv[1], "--skip",6) == 0){
      printf("come in\n");
      char *arg[] = {"/bin/pal", "--skip", NULL};
      context_uload(current, fname, arg, envp);
  }else{
    context_uload(current, fname, argv, envp);
  }
  switch_boot_pcb();
  yield();
  return -1;
}

void do_syscall(Context *c) {
  uintptr_t a[4];

  a[0] = c->GPR1;
  
  /*================strace================*/
  //Log("Name: %s\t SYSID: %d\t Return %d\t",getSystemCallString(a[0]),c->GPR1,c->GPRx); 
  
  intptr_t ret = 0;
  switch (a[0]) {
    case Trap: Log("Trap");break; //deal with trap
    case SYS_exit:/*Log("handle sys_exit");*/sys_exit(c->GPRx); break;
    case SYS_yield:/*Log("handle sys_yield");*/ret = sys_yield();break;
    case SYS_brk: /*Log("handle sys_brk");*/ret = sys_brk((void*)c->GPR2);break;
    /*============about fs operate========*/
    case SYS_write: /*Log("handle sys_write");*/ret = sys_write(c->GPR2,(void*)c->GPR3,(size_t)c->GPR4);break;
    case SYS_read: /*Log("handle sys_read");*/ret = sys_read(c->GPR2,(void*)c->GPR3,(size_t)c->GPR4);break;
    case SYS_open: /*Log("handle sys_open");*/ret = sys_open((const char *)c->GPR2);break;
    case SYS_close: /*Log("handle sys_close");*/ret = sys_close(c->GPR2);break;
    case SYS_lseek: /*Log("handle sys_lseek");*/ret = sys_lseek(c->GPR2,(off_t)c->GPR3,c->GPR4);break;
    case SYS_execve: /*Log("handle sys_execve");*/ret = sys_execve((const char *)c->GPR2, (char * const*)c->GPR3, (char * const*)c->GPR4);break;
    /*============about device============*/
    case SYS_gettimeofday: /*Log("handle sys_gettimeofday");*/ret = sys_gettimeofday((struct timeval *)c->GPR2,(struct timezone *)c->GPR3);break;
    /*============other===================*/
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  
  c->GPRx = ret;
}

