#include <common.h>
#include "syscall.h"

int sys_yield() {
    yield();
    return 0;
}

void sys_exit(int status) {
    halt(status);
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  
  a[0] = c->GPR1;

  intptr_t ret;
  switch (a[0]) {
    case Trap: Log("Trap ");break; //deal with trap
    case SYS_exit:Log("handle sys_exit");sys_exit(c->GPRx); break;
    case SYS_yield: Log("handle sys_yield");ret = sys_yield();break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  
  c->GPRx = ret;
}

