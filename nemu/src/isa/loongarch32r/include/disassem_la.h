#ifndef __DISASSEM_LA__
#define __DISASSEM_LA__
    #include <isa-def.h>
    #include "../local-include/reg.h"

    #define PCADDU12I 0x0e
    #define ST_W      0xA6
    #define ST_H      0xA5
    #define ST_B      0xA4  
    #define LD_W      0xA2
    #define Break     0x54
    #define SYSCALL   0x56
    #define ADD_W     0x20
    #define SUB_W     0x22
    #define ADDI_W    0x0A
    #define LU12I_W   0x0A
    #define STL       0x24
    #define SLTU      0x25
    #define SLTI      0x08
    #define SLTUI     0x09
    #define NOR       0x28
    #define AND       0x29
    #define OR        0x2A
    #define XOR       0x2B
    #define ANDI      0x0D
    #define ORI       0x0E
    #define XORI      0x0F
    #define NOP       ANDI
    #define MUL_W     0X38
    #define MULH_W    0X39
    #define MULH_WU   0x3A
    #define DIV_W     0x40
    #define DIV_WU    0x42
    #define MOD_W     0x41
    #define MOD_WU    0x43
    #define SLL_W     0x2E
    #define SRL_W     0x2F
    #define SRA_W     0x30
    #define BEQ       0x16
    #define BNE       0x17
    #define BLT       0x18
    #define BLTU      0x1A
    #define BGEU      0x1B
    #define BGE       0x19
    #define B         0x14
    #define BL        0x15
    #define JIRL      0x13
    #define LD_B      0xA0
    #define LD_H      0xA1
    #define LD_W      0xA2
    #define LD_BU     0xA8
    #define LD_HU     0xA9
    #define LL_W      0x20
    #define SC_W      0x21
    #define DBAR      0x70E4
    #define IBAR      0x70E5
    #define RDCNTID_W 0x18
    #define RDCNTVL_W 0x18
    #define RDCNTVH_W 0x19
    #define SLLI_W    0x81
    #define SRLI_W    0x89
    #define SRAI_W    0x91
    #define CSRRD     0x04
    #define CSRWR     0x04
    #define CSRXCHG   0x04 
    #define CAROP     0x18
    #define TLBSRCH   0x1920A
    #define TLBRD     0x1920B
    #define TLBWR     0x1920C
    #define TLBFILL   0x1920D
    #define ERTN      0x1920E
    #define IDLE      0xC91
    #define INVTLB    0xC93
    #define PRELD     0xAB

#endif
