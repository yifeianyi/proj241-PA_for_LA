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
    Log("Hello World from Nanos-lite with arg '%s' for the %dth time!", (intptr_t)arg, j);
    j ++;
    yield();
  }
}

/*============new insert============*/
void context_kload(PCB *pcb,void (*func)(void*), void *arg){
  //创建上下文
  Area stack;
  stack.start = pcb->stack;
  stack.end = pcb->stack + STACK_SIZE;
  //将创建完成的返回的指针记录到pcb的cp中
  pcb->cp = kcontext(stack, func, arg); 
}

/*============new insert============*/
void context_uload(PCB *pcb, const char *filename,char *const argv[],char *const envp[]){
  Log("filename: %s", filename);
  uintptr_t entry = uloader(pcb, filename);

  Area stack;
  stack.start = pcb->stack;
  stack.end = pcb->stack + STACK_SIZE;

  Log("stack.start: %p, stack.end: %p", stack.start, stack.end);
  Log("entry: %p", entry);

  // prepare entry point
  pcb->cp = ucontext(NULL, stack, (void(*)()) entry);

  // prepare argv and envp
  void *ustack_end = new_page(8);
  int space_count = sizeof(uintptr_t); // for argc
  int argc = 0;
  int envpc = 0;

  // Calculate space for argv
  if (argv) {
      while (argv[argc]) {
          space_count += sizeof(uintptr_t) + strlen(argv[argc]) + 1;
          argc++;
      }
  }
  space_count += sizeof(uintptr_t); // for terminating NULL of argv

  // Calculate space for envp
  if (envp) {
      while (envp[envpc]) {
          space_count += sizeof(uintptr_t) + strlen(envp[envpc]) + 1;
          envpc++;
      }
  }
  space_count += sizeof(uintptr_t); // for terminating NULL of envp

  Log("argc: %d, envpc: %d, space_count: %d", argc, envpc, space_count);

  // Allocate space on stack
  uintptr_t *base = (uintptr_t *)ROUNDUP(ustack_end - space_count, sizeof(uintptr_t));
  uintptr_t *base_mem = base;
  Log("base after ROUNDUP: %p", base);

  *base = argc;
  base++;

  // Allocate space for argv and envp pointers
  char *argv_temp[argc];
  char *envp_temp[envpc];
  uintptr_t *arg_start = base + argc + 1;
  uintptr_t *env_start = arg_start + envpc + 1;

  // Copy argv strings to stack
  for (int i = 0; i < argc; i++) {
      argv_temp[i] = (char *)arg_start;
      strcpy((char *)arg_start, argv[i]);
      arg_start += strlen(argv[i]) + 1;
      Log("argv[%d]: %s, addr: %p", i, argv[i], argv_temp[i]);
  }

  // Copy envp strings to stack
  for (int i = 0; i < envpc; i++) {
      envp_temp[i] = (char *)env_start;
      strcpy((char *)env_start, envp[i]);
      env_start += strlen(envp[i]) + 1;
      Log("envp[%d]: %s, addr: %p", i, envp[i], envp_temp[i]);
  }

  // Set argv pointers in stack
  for (int i = 0; i < argc; i++) {
      *base++ = (uintptr_t)argv_temp[i];
  }
  *base++ = (uintptr_t)NULL;

  // Set envp pointers in stack
  for (int i = 0; i < envpc; i++) {
      *base++ = (uintptr_t)envp_temp[i];
  }
  *base = (uintptr_t)NULL;

  // Validate stack alignment
  assert((char *)base + sizeof(uintptr_t) == (char *)env_start);

  pcb->cp->GPRx = (uintptr_t)base_mem;
}


void init_proc() {
  //char *A = "M";
  char *B = "B";
  //char *argv[] = {"/bin/pal", "--skip", NULL};
  //printf("%s %s\n",argv[0],argv[1]);
  context_kload(&pcb[0], hello_fun,(void*)B);
  //context_uload(&pcb[1], "/bin/pal", argv, NULL);
  char *argv[] = {"/bin/nterm",NULL,NULL};
  //char *argv[] = {"/bin/menu",NULL,NULL};
  //char *argv[] = {"/bin/exec-test",NULL,NULL};
  //context_uload(&pcb[1],"/bin/exec-test",argv,NULL);
  //context_uload(&pcb[1],"/bin/menu",argv,NULL);
  context_uload(&pcb[1],"/bin/nterm",argv,NULL);
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
