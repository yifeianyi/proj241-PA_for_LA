#include <proc.h>
#include </home/lazy/proj241-PA_for_LA/abstract-machine/am/include/am.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;


void switch_boot_pcb() {
  current = &pcb_boot;
}

void hello_fun(void *arg) {
  int j = 1;
  while (1) {
    Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", (uintptr_t)arg, j);
    j ++;
    yield();
  }
}

/*============new insert============*/
void context_kload(PCB *pcb,void (*func)(void*), void *arg){
  //创建上下文
  Area stack;
  stack.start = pcb->stack;
  stack.end = pcb->stack + 36;
  Log("%p %p\n",stack.start,stack.end); 
  //将创建完成的返回的指针记录到pcb的cp中
  pcb->cp = kcontext(stack, func, arg); 
}

void init_proc() {
  context_kload(&pcb[0], hello_fun, "A");
  context_kload(&pcb[1], hello_fun, "B");
  switch_boot_pcb();

  Log("Initializing processes...");

  // load program here
  //const char filename[] = "/bin/dummy";
  //const char filename[] = "/bin/file-test";
  //const char filename[] = "/bin/timer-test";
  //const char filename[] = "/bin/exec-test";
  //const char filename[] = "/bin/event-test";
  //const char filename[] = "/bin/vga-test";
  //const char filename[] = "/bin/nslider";
  //const char filename[] = "/bin/menu";
  //const char filename[] = "/bin/nterm";
  //const char filename[] = "/bin/bird";
  //const char filename[] = "/bin/pal";
  //naive_uload(NULL, filename);
}

Context* schedule(Context *prev) {
  current->cp = prev;
  current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
  return current->cp;
}
