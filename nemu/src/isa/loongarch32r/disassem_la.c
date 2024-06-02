#include<disassem_la.h>
#include <isa.h>
extern const char *regs[];
static char* csr_name(int csr_id){
    switch (csr_id)
    {
    case CSR_CRMD: return "CRMD";
    case CSR_PRMD: return "PRMD";
    case CSR_ESTAT:return "ESTAT";
    case CSR_ERA:  return "ERA";
    case CSR_EENTRY:return "EENTRY";
    default:return NULL;
    }
}
static uint32_t GetInst(char *inst, uint32_t code){
    uint32_t opcode_31_10 = BITS(code, 31, 10);
    uint32_t opcode_31_15 = BITS(code, 31, 15);
    uint32_t opcode_31_22 = BITS(code, 31, 22);
    uint32_t opcode_31_24 = BITS(code, 31, 24);
    uint32_t opcode_31_25 = BITS(code, 31, 25);
    uint32_t opcode_31_26 = BITS(code, 31, 26);
    uint32_t opcode_4_0   = BITS(code,  4,  0);
    uint32_t opcode_csr   = BITS(code,  9,  5);

    if (opcode_31_25 == PCADDU12I){strcpy(inst, "pcaddu12i");return TYPE_1RI20;}
    else if(opcode_31_25 == LU12I_W){strcpy(inst, "lu12i.w");return TYPE_1RI20;}

    else if(opcode_31_22 == ST_W){strcpy(inst, "st.w");return TYPE_2RI12;}
    else if(opcode_31_22 == ST_B){strcpy(inst, "st.b");return TYPE_2RI12;}
    else if(opcode_31_22 == ST_H){strcpy(inst, "st.h");return TYPE_2RI12;}
    else if(opcode_31_22 == ADDI_W){strcpy(inst, "addi.w");return TYPE_2RI12;}
    else if(opcode_31_22 == SLTI){strcpy(inst, "slti");return TYPE_2RI12;}
    else if(opcode_31_22 == SLTUI){strcpy(inst, "sltui");return TYPE_2RI12;}
    else if(opcode_31_22 == ANDI){strcpy(inst, "andi");return TYPE_2RI12;}
    else if(opcode_31_22 == ORI){strcpy(inst,"ori");return TYPE_2RUI12;}
    else if(opcode_31_22 == XORI){strcpy(inst, "xori");return TYPE_2RUI12;}
    else if(opcode_31_22 == LD_B){strcpy(inst, "ld.b");return TYPE_2RI12;}
    else if(opcode_31_22 == LD_H){strcpy(inst, "ld.h");return TYPE_2RI12;}
    else if(opcode_31_22 == LD_W){strcpy(inst, "ld.w");return TYPE_2RI12;}
    else if(opcode_31_22 == LD_BU){strcpy(inst,"ld.bu");return TYPE_2RI12;}
    else if(opcode_31_22 == LD_HU){strcpy(inst, "ld.hu");return TYPE_2RI12;}
    else if(opcode_31_22 == CAROP){strcpy(inst, "cacop");return TYPE_CPROP;}
    else if(opcode_31_22 == PRELD){strcpy(inst, "preld");return TYPE_PRELD;}

    else if(opcode_31_15 == ADD_W){strcpy(inst, "add.w");return TYPE_3R;}
    else if(opcode_31_15 == SUB_W){strcpy(inst, "sub.w");return TYPE_3R;}
    else if(opcode_31_15 == STL){strcpy(inst, "stl");return TYPE_3R;}
    else if(opcode_31_15 == SLTU){strcpy(inst, "stlu");return TYPE_3R;}
    else if(opcode_31_15 == NOR){strcpy(inst, "nor");return TYPE_3R;}
    else if(opcode_31_15 == AND){strcpy(inst, "and");return TYPE_3R;}
    else if(opcode_31_15 == OR){strcpy(inst, "or");return TYPE_3R;}
    else if(opcode_31_15 == XOR){strcpy(inst, "xor");return TYPE_3R;}
    else if(opcode_31_15 == MUL_W){strcpy(inst, "mul.w");return TYPE_3R;}
    else if(opcode_31_15 == MULH_W){strcpy(inst, "mulh.w");return TYPE_3R;}
    else if(opcode_31_15 == MULH_WU){strcpy(inst, "mulh.wu");return TYPE_3R;}
    else if(opcode_31_15 == DIV_W){strcpy(inst, "div.w");return TYPE_3R;}
    else if(opcode_31_15 == DIV_WU){strcpy(inst, "div.wu");return TYPE_3R;}
    else if(opcode_31_15 == MOD_W){strcpy(inst, "mod.w");return TYPE_3R;}
    else if(opcode_31_15 == MOD_WU){strcpy(inst, "mod.wu");return TYPE_3R;}
    else if(opcode_31_15 == SLL_W){strcpy(inst, "sll.w");return TYPE_3R;}
    else if(opcode_31_15 == SRL_W){strcpy(inst, "srl.w");return TYPE_3R;}
    else if(opcode_31_15 == SRA_W){strcpy(inst, "sra.wu");return TYPE_3R;}
    else if(opcode_31_15 == DBAR){strcpy(inst, "dbar");return TYPE_LANZAN;}
    else if(opcode_31_15 == IBAR){strcpy(inst, "ibar");return TYPE_LANZAN;}
    else if(opcode_31_15 == IDLE){strcpy(inst, "idle");return TYPE_IDLE;}
    else if(opcode_31_15 == INVTLB){strcpy(inst, "invtlb");return TYPE_INVTLB;}

    else if(opcode_31_26 == BEQ){strcpy(inst, "beq");return TYPE_2RI16;}
    else if(opcode_31_26 == BNE){strcpy(inst, "bne");return TYPE_2RI16;}
    else if(opcode_31_26 == BLT){strcpy(inst, "blt");return TYPE_2RI16;}
    else if(opcode_31_26 == BLTU){strcpy(inst, "bltu");return TYPE_2RI16;}
    else if(opcode_31_26 == JIRL){strcpy(inst, "jirl");return TYPE_2RI16;}
    else if(opcode_31_26 == BGE){strcpy(inst, "bge");return TYPE_2RI16;}
    else if(opcode_31_26 == BGEU){strcpy(inst, "bgeu");return TYPE_2RI16;}

    else if(opcode_31_26 == B){strcpy(inst, "b");return TYPE_I26;}
    else if(opcode_31_26 == BL){strcpy(inst, "bl");return TYPE_I26;}


    else if(opcode_31_24 == LL_W){strcpy(inst, "ll.w");return TYPE_2RI14;}
    else if(opcode_31_24 == SC_W){strcpy(inst, "sc.w");return TYPE_2RI14;}

    /*modify finish*/
    else if(opcode_31_24 == CSRRD && opcode_csr == 0){strcpy(inst, "csrrd"); return TYPE_CSR;}
    else if(opcode_31_24 == CSRWR && opcode_csr == 1){strcpy(inst, "csrwr"); return TYPE_CSR;}
    else if(opcode_31_24 == CSRXCHG && (opcode_csr != 1 && opcode_csr == 0)){strcpy(inst, "csrxchg"); return TYPE_CSR;}


    else if(opcode_31_15 == Break){strcpy(inst, "break");return TYPE_S;}
    else if(opcode_31_15 == SYSCALL){strcpy(inst, "syscall");return TYPE_S;}

    /*modify finish*/
    else if(opcode_31_10 == RDCNTVL_W && opcode_csr == 0 ){strcpy(inst, "rdcntvl.w");return TYPE_1R;}
    else if(opcode_31_10 == RDCNTID_W && opcode_4_0 == 0){strcpy(inst, "rdcntid.w");return TYPE_1R;}
    else if(opcode_31_10 == RDCNTVH_W && opcode_csr == 0){strcpy(inst, "rdcntvh.w");return TYPE_1R;}

    else if(opcode_31_10 == TLBSRCH){strcpy(inst, "tlbsrch");return NONE_TYPE;}
    else if(opcode_31_10 == TLBRD){strcpy(inst, "tlbrd");return NONE_TYPE;}
    else if(opcode_31_10 == TLBWR){strcpy(inst, "tlbwr");return NONE_TYPE;}
    else if(opcode_31_10 == TLBFILL){strcpy(inst, "tlbfill");return NONE_TYPE;}
    else if(opcode_31_10 == ERTN){strcpy(inst, "ertn");return NONE_TYPE;}

    else if(opcode_31_15 == SLLI_W){strcpy(inst, "slli.w");return TYPE_2RUI5;}
    else if(opcode_31_15 == SRLI_W){strcpy(inst, "srli.w");return TYPE_2RUI5;}
    else if(opcode_31_15 == SRAI_W){strcpy(inst, "srai.w");return TYPE_2RUI5;}

    else { strcpy(inst,"\0");return TYPE_N;}
}
void disassem_la(char *str, uint32_t code){
        //获取指令
        char inst[30]="";
        uint32_t type =  GetInst(inst,code);
        Assert(inst!=NULL,"GetInst false.");

        char p[30];
        uint8_t rd = 0;
        uint8_t rj = 0;
        uint8_t rk = 0;
        uint8_t csr_id = 0;
        uint16_t hint = 0;
        uint8_t op = 0;
        rj = BITS(code, 9, 5);
        rd = BITS(code, 4, 0);
        rk = BITS(code, 14,10);
        
        uint32_t uimm = 0;
        int imm = 0;
        int offs = 0;
        int level = 0;
        switch (type)
        {
        case TYPE_1RI20:
            imm = SEXT(BITS(code, 24, 5), 20) << 12;
            sprintf(p,"  %s, %d",regs[rd],imm);
            break;
        case TYPE_2RI12:
            imm = SEXT(BITS(code, 21, 10),12);
            sprintf(p,"  %s,%s,%d",regs[rd],regs[rj],(int32_t)imm);
            break;
        case TYPE_2RUI12:
            uimm = BITS(code, 21, 10);
            sprintf(p,"  %s,%s,%d",regs[rd],regs[rj],uimm);
            break;
        case TYPE_CPROP:
            imm = BITS(code, 21, 10);
            code = BITS(code, 4, 0);
            sprintf(p,"  %d,%s,%d",code,regs[rj],imm);
            break;
        case TYPE_PRELD:
            imm = BITS(code, 21, 10);
            hint = BITS(code, 4, 0);
            sprintf(p,"  %d,%s,%d",hint,regs[rj],imm);
            break;
        case TYPE_3R:
            sprintf(p,"  %s,%s,%s",regs[rd],regs[rj],regs[rk]);
            break;
        case TYPE_2RUI5:
            uimm = BITS(code, 14,10);
            sprintf(p,"  %s,%s,%d",regs[rd],regs[rj],uimm);
            break;
        case TYPE_1R:
            if(BITS(code,4,0) == 0){
                sprintf(p," %s",regs[rj]);
            }
            if(BITS(code,4,0) != 0 && BITS(code,9,5) == 0){
                sprintf(p," %s",regs[rd]);
            }
            break;
        case TYPE_2RI16:
            offs = BITS(code,25,10); 
            sprintf(p,"  %s,%s,%d",regs[rd],regs[rj],offs);
            break;
        case TYPE_2RI14:
            imm = BITS(code,23,10);
            sprintf(p,"  %s,%s,%d",regs[rd],regs[rj],imm);
            break;
        case TYPE_CSR:
            csr_id = BITS(code,23,10); 
            char *name = csr_name(csr_id);
            if(name==NULL)sprintf(name,"csr(%d)",csr_id);
            sprintf(p," $%s %s",regs[rd],name);
            break;
        case TYPE_S:
            imm = BITS(code,14,0);
            sprintf(p," %d",imm);
            break;
        case TYPE_I26:
            offs = SEXT(BITS(code,25,10)|(BITS(code,9,0)<<16),26);
            sprintf(p," %d(0x%x)",offs,offs);
            break;
        case TYPE_LANZAN:
            hint = BITS(code,14,0);
            sprintf(p," %d",hint);
            break;
        case TYPE_IDLE:
            level = BITS(code,14,0);
            sprintf(p," %d",level);
            break;
        case TYPE_INVTLB:
            op = BITS(code,4,0);
            sprintf(p," %d %s %s",op,regs[rj],regs[rk]);
            break;
        default:
            sprintf(p,"%c",'\0');
            break;
        }
        strcat(inst,p);
        strcpy(str,inst);
}

