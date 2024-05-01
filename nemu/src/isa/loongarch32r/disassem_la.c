#include<disassem_la.h>
#include <isa.h>
extern const char *regs[];
static uint32_t GetInst(char *inst, uint32_t code){
    uint32_t opcode_31_10 = BITS(code, 31, 10);
    uint32_t opcode_31_15 = BITS(code, 31, 15);
    // uint32_t opcode_31_18 = BITS(code, 31, 18);
    // uint32_t opcode_31_20 = BITS(code, 31, 20);
    uint32_t opcode_31_22 = BITS(code, 31, 22);
    uint32_t opcode_31_24 = BITS(code, 31, 24);
    uint32_t opcode_31_25 = BITS(code, 31, 25);
    uint32_t opcode_31_26 = BITS(code, 31, 26);
    
    if (opcode_31_25 == PCADDU12I){strcpy(inst, "pcaddu12i");return TYPE_1RI20;}
    else if(opcode_31_25 == LU12I_W){strcpy(inst, "LU12I.w");return TYPE_1RI20;}

    else if(opcode_31_22 == ST_W){strcpy(inst, "ST.w");return TYPE_2RI12;}
    else if(opcode_31_22 == ST_B){strcpy(inst, "ST.B");return TYPE_2RI12;}
    else if(opcode_31_22 == ST_H){strcpy(inst, "ST.H");return TYPE_2RI12;}
    else if(opcode_31_22 == ADDI_W){strcpy(inst, "ADDI.w");return TYPE_2RI12;}
    else if(opcode_31_22 == SLTI){strcpy(inst, "SLTI");return TYPE_2RI12;}
    else if(opcode_31_22 == SLTUI){strcpy(inst, "SLTUI");return TYPE_2RI12;}
    else if(opcode_31_22 == ANDI){strcpy(inst, "ANDI");return TYPE_2RI12;}
    else if(opcode_31_22 == ORI){strcpy(inst,"ORI");return TYPE_2RI12_U;}
    else if(opcode_31_22 == XORI){strcpy(inst, "XORI");return TYPE_2RI12_U;}
    else if(opcode_31_22 == LD_B){strcpy(inst, "LD.B");return TYPE_2RI12;}
    else if(opcode_31_22 == LD_H){strcpy(inst, "LD.H");return TYPE_2RI12;}
    else if(opcode_31_22 == LD_W){strcpy(inst, "LD.W");return TYPE_2RI12;}
    else if(opcode_31_22 == LD_BU){strcpy(inst,"LD.BU");return TYPE_2RI12;}
    else if(opcode_31_22 == LD_HU){strcpy(inst, "LD.HU");return TYPE_2RI12;}
    else if(opcode_31_22 == CAROP){strcpy(inst, "CACOP");return TYPE_2RI12_CP;}
    else if(opcode_31_22 == PRELD){strcpy(inst, "PRELD");return TYPE_2RI12_PR;}

    else if(opcode_31_15 == ADD_W){strcpy(inst, "ADD.w");return TYPE_3R;}
    else if(opcode_31_15 == SUB_W){strcpy(inst, "SUB.w");return TYPE_3R;}
    else if(opcode_31_15 == STL){strcpy(inst, "STL");return TYPE_3R;}
    else if(opcode_31_15 == SLTU){strcpy(inst, "STLU");return TYPE_3R;}
    else if(opcode_31_15 == NOR){strcpy(inst, "NOR");return TYPE_3R;}
    else if(opcode_31_15 == AND){strcpy(inst, "AND");return TYPE_3R;}
    else if(opcode_31_15 == OR){strcpy(inst, "OR");return TYPE_3R;}
    else if(opcode_31_15 == XOR){strcpy(inst, "XOR");return TYPE_3R;}
    else if(opcode_31_15 == MUL_W){strcpy(inst, "MUL.W");return TYPE_3R;}
    else if(opcode_31_15 == MULH_W){strcpy(inst, "MULH.W");return TYPE_3R;}
    else if(opcode_31_15 == MULH_WU){strcpy(inst, "MULH.WU");return TYPE_3R;}
    else if(opcode_31_15 == DIV_W){strcpy(inst, "DIV.W");return TYPE_3R;}
    else if(opcode_31_15 == DIV_WU){strcpy(inst, "DIV.WU");return TYPE_3R;}
    else if(opcode_31_15 == MOD_W){strcpy(inst, "MOD.W");return TYPE_3R;}
    else if(opcode_31_15 == MOD_WU){strcpy(inst, "MOD.WU");return TYPE_3R;}
    else if(opcode_31_15 == SLL_W){strcpy(inst, "SLL.W");return TYPE_3R;}
    else if(opcode_31_15 == SRL_W){strcpy(inst, "SRL.W");return TYPE_3R;}
    else if(opcode_31_15 == SRA_W){strcpy(inst, "SRA.WU");return TYPE_3R;}
    else if(opcode_31_15 == DBAR){strcpy(inst, "DBAR");return TYPE_LANZAN;}
    else if(opcode_31_15 == IBAR){strcpy(inst, "IBAR");return TYPE_LANZAN;}
    else if(opcode_31_15 == IDLE){strcpy(inst, "IDLE");return TYPE_ID;}
    else if(opcode_31_15 == INVTLB){strcpy(inst, "INVTLB");return TYPE_IN;}

    else if(opcode_31_26 == BEQ){strcpy(inst, "BEQ");return TYPE_2RI16;}
    else if(opcode_31_26 == BNE){strcpy(inst, "BNE");return TYPE_2RI16;}
    else if(opcode_31_26 == BLT){strcpy(inst, "BLT");return TYPE_2RI16;}
    else if(opcode_31_26 == BLTU){strcpy(inst, "BLTU");return TYPE_2RI16;}
    else if(opcode_31_26 == JIRL){strcpy(inst, "JIRL");return TYPE_2RI16;}
    else if(opcode_31_26 == BGE){strcpy(inst, "BGE");return TYPE_2RI16;}
    else if(opcode_31_26 == BGEU){strcpy(inst, "BGEU");return TYPE_2RI16;}

    else if(opcode_31_26 == B){strcpy(inst, "B");return TYPE_I26;}
    else if(opcode_31_26 == BL){strcpy(inst, "BL");return TYPE_I26;}

    else if(opcode_31_24 == LL_W){strcpy(inst, "LL.W");return TYPE_2RI14;}
    else if(opcode_31_24 == SC_W){strcpy(inst, "SC.W");return TYPE_2RI14;}
    else if(opcode_31_24 == CSRRD){return TYPE_2RI14_1;}
    else if(opcode_31_24 == CSRWR){return TYPE_2RI14_1;}
    else if(opcode_31_24 == CSRXCHG){return TYPE_2RI14_1;}

    else if(opcode_31_15 == Break){strcpy(inst, "break");return TYPE_S;}
    else if(opcode_31_15 == SYSCALL){strcpy(inst, "syscall");return TYPE_S;}

    else if(opcode_31_10 == RDCNTVL_W){strcpy(inst, "");return TYPE_2R;}
    else if(opcode_31_10 == RDCNTID_W){strcpy(inst, "");return TYPE_2R;}
    else if(opcode_31_10 == RDCNTVH_W){strcpy(inst, "RDCNTVH.W");return TYPE_2R_1;}
    else if(opcode_31_10 == TLBSRCH){strcpy(inst, "TLBSRCH");return NONE_TYPE;}
    else if(opcode_31_10 == TLBRD){strcpy(inst, "TLBRD");return NONE_TYPE;}
    else if(opcode_31_10 == TLBWR){strcpy(inst, "TLBWR");return NONE_TYPE;}
    else if(opcode_31_10 == TLBFILL){strcpy(inst, "TLBFILL");return NONE_TYPE;}
    else if(opcode_31_10 == ERTN){strcpy(inst, "ERTN");return NONE_TYPE;}

    else if(opcode_31_15 == SLLI_W){strcpy(inst, "SLLI.W");return TYPE_3R_U;}
    else if(opcode_31_15 == SRLI_W){strcpy(inst, "SRLI.W");return TYPE_3R_U;}
    else if(opcode_31_15 == SRAI_W){strcpy(inst, "SRAI.W");return TYPE_3R_U;}

    else { strcpy(inst,"\0");return TYPE_N;}
}
void disassem_la(char *str, uint64_t pc, uint32_t code){
        
        char inst[30];
        uint32_t type =  GetInst(inst,code);
        Assert(inst!=NULL,"GetInst false.");

        char p[20];
        uint8_t rd = 0;
        uint8_t rj = 0;
        uint8_t rk = 0;
        uint8_t csr = 0;
        uint16_t hint = 0;
        uint8_t op = 0;
        rj = SEXT(BITS(code, 9, 5),5);
        rd = SEXT(BITS(code, 4, 0),5);
        rk = SEXT(BITS(code, 14,10),5);
        
        uint32_t uimm = 0;
        int imm = 0;
        int offs = 0;
        int level = 0;
        switch (type)
        {
        case TYPE_1RI20:
            imm = SEXT(BITS(code, 24, 5), 20) << 12;
            sprintf(p,"  $%s, %d",regs[rd],imm);
            break;
        case TYPE_2RI12:
            imm = SEXT(BITS(code, 21, 10), 12);
            sprintf(p,"  $%s,$%s,%d",regs[rd],regs[rj],imm);
            break;
        case TYPE_2RI12_U:
            uimm = SEXT(BITS(code, 21, 10), 12);
            sprintf(p,"  $%s,$%s,%d",regs[rd],regs[rj],uimm);
            break;
        case TYPE_2RI12_CP:
            imm = SEXT(BITS(code, 21, 10), 12);
            code = BITS(code, 4, 0);
            sprintf(p,"  %d,$%s,%d",code,regs[rj],imm);
            break;
        case TYPE_2RI12_PR:
            imm = SEXT(BITS(code, 21, 10), 12);
            hint = SEXT(BITS(code, 4, 0),5);
            sprintf(p,"  %d,$%s,%d",hint,regs[rj],imm);
            break;
        case TYPE_3R:
            sprintf(p,"  $%s,$%s,$%s",regs[rd],regs[rj],regs[rk]);
            break;
        case TYPE_3R_U:
            uimm = SEXT(BITS(code, 14,10),5);
            sprintf(p,"  $%s,$%s,%d",regs[rd],regs[rj],uimm);
            break;
        case TYPE_2R:
            if(BITS(code,4,0) == 0){
                sprintf(p,"RDCNTID.W  $%s",regs[rj]);
            }
            if(BITS(code,4,0) != 0 && BITS(code,9,5) == 0){
                sprintf(p,"RDCNTVL.W  $%s",regs[rd]);
            }
            break;
        case TYPE_2R_1:
            sprintf(p,"  $%s",regs[rd]);
            break;
        case TYPE_2RI16:
            offs = SEXT(BITS(code,25,10),15); 
            sprintf(p,"  $%s,$%s,%d",regs[rj],regs[rd],offs);
            break;
        case TYPE_2RI14:
            imm = SEXT(BITS(code,23,10),14);
            sprintf(p,"  $%s,$%s,%d",regs[rd],regs[rj],imm);
            break;
        case TYPE_2RI14_1:
            if(BITS(code,9,5) == 0){
                csr = SEXT(BITS(code,23,10),14); 
                sprintf(p,"CSRRD $%s $%s",regs[rd],regs[csr]);
            }else if(BITS(code,9,5) == 1){
                csr = SEXT(BITS(code,23,10),14);
                sprintf(p,"CSRWR $%s $%s",regs[rd],regs[csr]);
            }else{
                csr = SEXT(BITS(code,23,10),14);
                sprintf(p,"CSRXCHG $%s $%s",regs[rd],regs[csr]);
            }
            break;
        case TYPE_S:
            imm = SEXT(BITS(code,14,0),15);
            sprintf(p," %d",imm);
            break;
        case TYPE_I26:
            offs = SEXT(BITS(code,25,10),15)+(BITS(code,9,0)<<15);
            sprintf(p," %d",offs);
            break;
        case TYPE_LANZAN:
            hint = BITS(code,14,0);
            sprintf(p," %d",hint);
            break;
        case TYPE_ID:
            level = SEXT(BITS(code,14,0),15);
            sprintf(p," %d",level);
            break;
        case TYPE_IN:
            op = SEXT(BITS(code,4,0),5);
            sprintf(p," %d $%s $%s",op,regs[rj],regs[rk]);
            break;
        default:
            sprintf(p,"%c",'\0');
            break;
        }
        strcat(inst,p);
        strcpy(str,inst);
}