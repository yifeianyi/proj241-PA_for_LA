#ifndef __DISASSEM_LA__
#define __DISASSEM_LA__
    #include<common.h>
    // enum{
    //     INST_ADDW, INST_SUBW, INST_ADDIW, INST_LU12IW,
    //     INST_SLT,  INST_SLTU, INST_SLTI,  INST_SLTUI,
    //     INST_PCADDU12I, INST_AND, INST_OR, INST_NOR, INST_XOR,
    //     INST_ANDI, INST_ORI, INST_XORI, INST_NOP,
    //     INST_MULW, INST_MULHW, INST_MULHWU,
    //     INST_DIVW, INST_MODW, INST_DIVWU, INST_MODWU,
    //     INST_SLLW, INST_SRLW, INST_SRAW,
    //     INST_SLLIW, INST_SRLIW, INST_SRAIW,
    //     INST_BEQ, INST_BNE, INST_BLT, INST_BGE, INST_BLTU, INST_BGEU,
    //     INST_BL, INST_JIRL, INST_LDB, INST_LDH, INST_LDW, INST_LDBU, INST_LDHU,
    //     INST_STB, INST_STH, INST_STW, INST_SYSCALL, INST_BREAK,
    //     INST_CSRRD, INST_CSRWR, INST_CSRXCHG
        
    // }INST_NUM;
    // static char inst_name[][20] = 
    // {
    //     "add.w", "sub.w", "addi.w", "lu12i,w",
    //     "slt", "sltu", "slti", "sltui",
    //     "pcaddu12i", "and", "or", "nor", "xor",
    //     "andi", "ori", "xori", "nop",
    //     "mul.w", "mulh.w", "mulh.wu",
    //     "div.w", "mod.w", "div.wu", "mod.wu",
    //     "sll.w", "srl.w", "sra.w", 
    //     "slli.w", "srli.w", "srai.w",
    //     "beq", "bne", "blt", "bge", "bltu", "bgeu",
    //     "bl", "jirl", "ld.b", "ld.h", "ld.w", "ld.bu", "ld.hu",
    //     "st.b", "st.h", "st.w", "syscall", "break",
    //     "csrrd", "csrwr", "csrxchg"
    // };

    #define PCADDU12I 14

#endif