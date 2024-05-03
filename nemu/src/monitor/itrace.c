#include <trace.h>
#ifdef CONFIG_ITRACE_IRINGBUF
#define IRING_SIZE CONFIG_ITRACE_IRINGBUF_SIZE
struct Buffer{
    char logbuf[128];
    vaddr_t pc;
    uint32_t idx;
}buffer[IRING_SIZE];

static uint32_t tail = 0;
static vaddr_t CurPC;

void push_to_iringbuf(const char *logbuf,vaddr_t pc){
    CurPC = pc;
    buffer[tail].pc = pc;
    buffer[tail].idx = tail;
    memcpy(buffer[tail].logbuf , logbuf, sizeof(buffer[tail].logbuf));
    tail = (tail + 1) % IRING_SIZE;
}

void print_iringbuf(){
    Log("=============== iringbuf ==================");
    for(uint32_t i = 0; i < IRING_SIZE; i++){
        if(CurPC == buffer[i].pc && buffer[i].idx == (IRING_SIZE+tail-1) % IRING_SIZE ) 
            printf(ANSI_FMT("--> %s", ANSI_BG_GREEN)"\n",buffer[i].logbuf);
        else if(buffer[i].logbuf!=NULL && strlen(buffer[i].logbuf)!=0)printf("    %s\n",buffer[i].logbuf); 
        else break;
    }
}
#endif