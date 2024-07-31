#include <common.h>
#include "syscall.h"
#include <proc.h>
static Context* do_event(Event e, Context* c) {
  switch (e.event){
    case EVENT_SYSCALL:do_syscall(c);c = schedule(c);break;
    case EVENT_YIELD: Log("yield!");break;
    case EVENT_ERROR: printf("Error\n");break;
    default: panic("Unhandled event ID = %d", e.event);
  }
  
  return c;
}

void init_irq(void) {
  Log("Initializing interrupt/exception handler...");
  cte_init(do_event);
}
