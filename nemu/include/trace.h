#ifndef __TRACE__
#define __TRACE__

#include "./common.h"

#define word_t uint32_t
void push_to_iringbuf(const char *logbuf,paddr_t pc);
void print_iringbuf(void);
void mtrace(paddr_t addr, int len, word_t data,char op);
#undef word_t
#endif