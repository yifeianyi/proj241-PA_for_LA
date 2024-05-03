#ifndef __TRACE__
#define __TRACE__
#include <common.h>


void push_to_iringbuf(const char *logbuf,vaddr_t pc);
void print_iringbuf(void);
void mtrace(vaddr_t addr, int len, word_t data,char op);

#endif