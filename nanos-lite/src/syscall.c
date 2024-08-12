#include <common.h>
#include "syscall.h"
void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;

  switch (a[0]) {
    case SYS_exit:{
      c->GPRx = 0;
#ifdef CONFIG_STRACE
  printf("exit(code: 0x%x)\n",c->GPRx);
#endif
      halt(c->GPRx); 
      break;
    } 
    case SYS_yield: c->GPRx = 0; yield();       break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
}
