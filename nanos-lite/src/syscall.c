#include <common.h>
#include "syscall.h"
#include <sys/time.h>
#include <fs.h>
#include <proc.h>
int Syscall_gettimeofday(struct timeval *tv, struct timezone *tz) {
    uint64_t us = io_read(AM_TIMER_UPTIME).us;
    tv->tv_sec = us / 1000000;
    tv->tv_usec = us - us / 1000000 * 1000000;
    return 0;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  // Log("======== In do_syscall ========== sysNO:%d",a[0]);
  switch (a[0]) {
    case SYS_brk:{
      c->GPRx = 0;break;
    }
    case SYS_exit:{
      c->GPRx = 0;halt(c->GPRx);break;
      #ifdef CONFIG_STRACE
        printf("exit(code: 0x%x)\n",c->GPRx);
      #endif
    } 
    case SYS_yield: c->GPRx = 0; yield(); break;
    case SYS_gettimeofday:{
      // Log("==== In gettimeofday ====");
      c->GPRx = Syscall_gettimeofday((struct timeval * )(c->GPR2),\
                                 (struct timezone *)(c->GPR3));
      break;
    }
    case SYS_open:{
      int fd = fs_open((const char *)c->GPR2, c->GPR3, c->GPR4);
      c->GPRx = fd;
      break;
    }
    case SYS_write:{
      #ifdef CONFIG_STRACE
        // Log("===== In sys_write =====");
        printf(ANSI_FMT("write(filename:%s, len: %d)",ANSI_BG_BLUE)"\n",GetFileName((int)c->GPR2),c->GPR4);
      #endif
      c->GPRx = fs_write((int)c->GPR2,(intptr_t *)c->GPR3,c->GPR4);
      break;
    }
    case SYS_read:{
      #ifdef CONFIG_STRACE
        // Log("===== In sys_write =====");
        printf(ANSI_FMT("read(filename:%s, len: %d)",ANSI_BG_BLUE)"\n",GetFileName((int)c->GPR2),c->GPR4);
      #endif
      
      c->GPRx = fs_read((int)(c->GPR2), (intptr_t *)(c->GPR3), (size_t)(c->GPR4));

      #ifdef CONFIG_STRACE
      if(strcmp("/proc/dispinfo",GetFileName((int)c->GPR2))){
        printf("buf:\n");
        for(int i = 0;i<c->GPR4;i++)putch(*(char*)(c->GPR3 + i));
        printf("\n");
      }
      #endif
      break;
    }
    case SYS_lseek:{
      c->GPRx = fs_lseek(c->GPR2, (size_t)c->GPR3, c->GPR4);
      break;
    }
    case SYS_close:{
      c->GPRx = fs_close((int)(c->GPR1));
      break;
    }
    case SYS_execve:{
      // extern void naive_uload(PCB *pcb, const char *filename);
      naive_uload(NULL, (const char *)c->GPR2);
      c->GPRx = 0;
    }
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
