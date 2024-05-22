#include <am.h>
#include <loongarch/loongarch32r.h>
#include <klib.h>

static Context* (*user_handler)(Event, Context*) = NULL;

Context* __am_irq_handle(Context *c) {
  if (user_handler) {
    Event ev = {0};
    uintptr_t ecode = c->estat >> 16;
    // printf("In __am_irq_handle,ecode:%d\n",ecode);
    switch (ecode) {
      case 10: ev.event = EVENT_SYSCALL;break;
      case 11: ev.event = EVENT_YIELD;break;
      default: ev.event = EVENT_ERROR; break;
    }
    c = user_handler(ev, c);
    assert(c != NULL);
  }

  c->era +=4;
  return c;
}

extern void __am_asm_trap(void);

bool cte_init(Context*(*handler)(Event, Context*)) {
  // initialize exception entry
  asm volatile("csrwr %0, 0xc" : : "r"(__am_asm_trap));  // 0xc = eentry

  // register event handler
  user_handler = handler;
  return true;
}

Context *kcontext(Area kstack, void (*entry)(void *), void *arg) {
  Context *context = (Context *)(kstack.end - sizeof(Context));
  context->era = (uintptr_t)entry;
  context->gpr[4] = (uintptr_t)arg;
  return context;
}

void yield() {
  asm volatile("li.w $a7, -1; syscall 0");
}

bool ienabled() {
  return false;
}

void iset(bool enable) {
}
