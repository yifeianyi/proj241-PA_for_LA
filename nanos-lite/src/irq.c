#include <common.h>
#include <syscall.h>
void do_syscall(Context *c);
static Context* do_event(Event e, Context* c) {
  switch (e.event){
    case EVENT_SYSCALL:printf("syscall\n");do_syscall(c);break;
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
