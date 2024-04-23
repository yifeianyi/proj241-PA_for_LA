#include<disassem_la.h>
extern const char *regs[];
static uint32_t GetInst(char *inst, uint32_t code){
    // uint32_t opcode_31_10 = BITS(code, 31, 10);
    // uint32_t opcode_31_15 = BITS(code, 31, 15);
    // uint32_t opcode_31_18 = BITS(code, 31, 18);
    // uint32_t opcode_31_20 = BITS(code, 31, 20);
    // uint32_t opcode_31_22 = BITS(code, 31, 22);
    // uint32_t opcode_31_24 = BITS(code, 31, 24);
    uint32_t opcode_31_25 = BITS(code, 31, 25);
    // uint32_t opcode_31_26 = BITS(code, 31, 26);

    if (opcode_31_25 == PCADDU12I) {strcpy(inst, "pcaddu12i");return TYPE_1RI21;}
    else { strcpy(inst,"\0");return TYPE_N;}
}
void disassem_la(char *str, uint64_t pc, uint32_t code){
        char inst[30];
        uint32_t type =  GetInst(inst,code);
        Assert(inst!=NULL,"GetInst false.");

        char p[20];
        int rj = BITS(code, 9, 5);
        switch (type)
        {
        case TYPE_1RI21:
            sprintf(p,"\t%s, ",regs[rj]);
            break;
        
        default:
            break;
        }
        strcat(inst,p);
        strcpy(str,inst);
}