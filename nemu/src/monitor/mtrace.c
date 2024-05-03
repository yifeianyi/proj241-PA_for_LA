#include "trace.h"

void mtrace(vaddr_t addr, int len, word_t data,char op){
    if(op=='r'){
        printf(ANSI_FMT("read "FMT_PADDR" %d byte. Data: "FMT_WORD"\n",ANSI_FG_YELLOW),addr,len,data);
    }
    else{
        printf(ANSI_FMT("write %d byte data into "FMT_PADDR". data:" FMT_WORD,ANSI_FG_CYAN)"\n",len, addr, data);
    }
}