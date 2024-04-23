#include<disassem_la.h>
/*****************************************************************************
 * 
 *  str:logbuf的指针
 *  code是八进制表示的指令
 *
******************************************************************************/

static void GetInst(char *inst, uint32_t code){
    // uint32_t opcode_31_10 = BITS(code, 31, 10);
    // uint32_t opcode_31_15 = BITS(code, 31, 15);
    // uint32_t opcode_31_18 = BITS(code, 31, 18);
    // uint32_t opcode_31_20 = BITS(code, 31, 20);
    // uint32_t opcode_31_22 = BITS(code, 31, 22);
    // uint32_t opcode_31_24 = BITS(code, 31, 24);
    uint32_t opcode_31_25 = BITS(code, 31, 25);
    // uint32_t opcode_31_26 = BITS(code, 31, 26);

    if (opcode_31_25 == PCADDU12I) {
        strcpy((char*)inst, "pcaddu12i\0");
        return ;
    }
    else {
        strcpy((char*)inst,"\0");
        return;
    }
}
void disassem_la(char *str, uint64_t pc, uint32_t code){
        // uint8_t p[20];
        char *inst = NULL;
        GetInst(inst,code);
        Assert(inst!=NULL,"GetInst false.");
        strcpy((char*)str,(char*)inst);
        printf("inst:%s",str);
}