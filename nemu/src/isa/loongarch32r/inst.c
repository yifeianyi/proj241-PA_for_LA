/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include "local-include/reg.h"
#include <cpu/cpu.h>
#include <cpu/ifetch.h>
#include <cpu/decode.h>
#include <isa.h>


#define src2R()  do { *src2 = R(rk); } while (0)
#define src1R()  do { *src1 = R(rj); } while (0)
#define simm12() do { *imm = SEXT(BITS(i, 21, 10),12); } while (0)
#define simm20() do { *imm = SEXT(BITS(i, 24, 5), 20) << 12; } while(0)
#define simm26() do { *imm = SEXT((BITS(i,25,10) | (BITS(i,9,0)<<16)),26); } while(0)
#define simm16() do { *imm = SEXT((BITS(i,25,10)<<2),32); } while (0)
#define uimm5()  do { *imm = SEXT(BITS(i,14,10),5);} while (0)
#define hint16() do { *imm = SEXT(BITS(i,15,0),16);} while(0)

static void decode_operand(Decode *s, int *rd_, word_t *src1, word_t *src2, word_t *imm, int type) {
  uint32_t i = s->isa.inst.val;
  int rj = BITS(i, 9, 5);
  int rk = BITS(i,14,10);
  *rd_ = BITS(i, 4, 0);
  switch (type) {
    case TYPE_1RI20:   simm20(); src1R();    break;
    case TYPE_2RI12:   simm12(); src1R();   break;
    case TYPE_2RUI12:  uimm12(); src1R();   break;
    case TYPE_3R:      src1R();  src2R();   break;
    case TYPE_I26:     simm26();            break;
    case TYPE_2RI16:   src1R();  simm16();  break;
    case TYPE_2RUI5:   simm5();  src1R();   break;
    case TYPE_LANZAN:  hint16();            break; 
  }
}

static int decode_exec(Decode *s) {
  int rd = 0;
  word_t src1 = 0, src2 = 0, imm = 0;
  uint64_t temp = 0;
  s->dnpc = s->snpc;

#define INSTPAT_INST(s) ((s)->isa.inst.val)
#define INSTPAT_MATCH(s, name, type, ... /* execute body */ ) { \
  decode_operand(s, &rd, &src1, &src2, &imm, concat(TYPE_, type)); \
  __VA_ARGS__ ; \
}
#define  GRLEN 32
  INSTPAT_START();
  INSTPAT("0001110 ????? ????? ????? ????? ?????" , pcaddu12i, 1RI20 , R(rd) = s->pc + SEXT((signed)(imm)<<12,GRLEN));
  INSTPAT("0001010 ????? ????? ????? ????? ?????" , LU12I.W  , 1RI20 , R(rd) = imm << 12);

  INSTPAT("0000000000 0100000????? ????? ?????"   , add.w    , 3R    , R(rd) = src1 + src2);
  INSTPAT("0000000000 0100010????? ????? ?????"   , sub.w    , 3R    , R(rd) = src1 - src2);
  INSTPAT("0000000000 1000000????? ????? ?????"   , DIV.W    , 3R    , R(rd) = (signed)src1 / (signed)src2);
  INSTPAT("0000000000 1000001????? ????? ?????"   , MOD.W    , 3R    , R(rd) = (signed)src1 % (signed)src2);
  
  INSTPAT("0000001010 ???????????? ????? ?????"   , ADDI.w   , 2RI12 , R(rd) = src1 + SEXT((signed)imm,32));//Log("R(%d):%08x, imm:%08x, R(X)=%08x",rd,R(rd),imm,imm+R(rd));
  INSTPAT("0000000000 0101010????? ????? ?????"   , or       ,  3R   , R(rd) = src1 | src2);

  INSTPAT("0000001110 ???????????? ????? ?????"   , ORI      , 2RUI12 , R(rd) = src1 | SEXT((unsigned)(imm & 0xFFF),32));
  INSTPAT("0000001101 ???????????? ????? ?????"   , ANDI     , 2RUI12 , R(rd) = src1 & SEXT((unsigned)(imm & 0xFFF),32));

  INSTPAT("010101???? ??????? ????? ????? ?????"  , bl       , I26   , R(1)  = s->pc + 4; s->dnpc = s->pc + SEXT(imm<<2, GRLEN); );//Log("Imm:"FMT_WORD"");

  INSTPAT("0010100010 ???????????? ????? ?????"   , ld.w     , 2RI12 , R(rd) = Mr(src1 + SEXT(imm,GRLEN), 4));
  INSTPAT("0010100001 ???????????? ????? ?????"   , ld.h     , 2RI12 , R(rd) = Mr(src1 + SEXT(imm,GRLEN), 2));
  INSTPAT("0010100000 ???????????? ????? ?????"   , ld.b     , 2RI12 , R(rd) = SEXT(Mr(src1 + SEXT(imm,GRLEN), 1),GRLEN));
  INSTPAT("0010101000 ???????????? ????? ?????"   , ld.bu    , 2RI12 , R(rd) = (unsigned)(Mr(src1 + SEXT(imm,GRLEN), 1)));
  INSTPAT("0010101001 ???????????? ????? ?????"   , ld.hu    , 2RI12 , R(rd) = (unsigned)(Mr(src1 + SEXT(imm,GRLEN), 2)));
  
  INSTPAT("0010100110 ???????????? ????? ?????"   , st.w     , 2RI12 , Mw(src1 + SEXT(imm,GRLEN), 4, R(rd)));
  INSTPAT("0010100101 ???????????? ????? ?????"   , st.h     , 2RI12 , Mw(src1 + SEXT(imm,GRLEN), 2, R(rd)&0xFFFF));
  INSTPAT("0010100100 ???????????? ????? ?????"   , st.b     , 2RI12 , Mw(src1 + SEXT(imm,GRLEN), 1, R(rd)&0xFF));


  INSTPAT("0000000000 1000010????? ????? ?????"   , DIV.WU   , 3R    , R(rd) = (unsigned)src1 / (unsigned)src2);
  INSTPAT("0000000000 1000001????? ????? ?????"   , MOD.WU   , 3R    , R(rd) = (unsigned)src1 % (unsigned)src2);
  
  INSTPAT("0000001001 ???????????? ????? ?????"   , SLTUI    , 2RI12 , if((uint32_t)src1 < (uint32_t)imm) R(rd) =1);
  INSTPAT("0000001001 ???????????? ????? ?????"   , SLTI     , 2RI12 , if(src1 < imm) R(rd) =1);
  
  
  INSTPAT("0000001111 ???????????? ????? ?????"   , XORI     , 2RUI12  , R(rd) = src1 ^ SEXT((unsigned)(imm),GRLEN));

  INSTPAT("0000000000 0111000????? ????? ?????"   , MUL.W    , 3R      , R(rd) = (signed)src1 * (signed)src2);
  INSTPAT("0000000000 0111001????? ????? ?????"   , MULH.W   , 3R      , R(rd) = (SEXT(((signed)src1 * (signed)src2),64)>>32) & 0xFFFFFFFF);
  INSTPAT("0000000000 0111001????? ????? ?????"   , MULH.WU  , 3R      , temp = (unsigned)src1 * (unsigned)src2;R(rd) = (temp>>32 & 0xFFFFFFFF));

  INSTPAT("0000000000 0101110????? ????? ?????"   , SLL.W    , 3R      , R(rd) = (unsigned)src1 << src2);
  INSTPAT("0000000000 0101111????? ????? ?????"   , SRL.W    , 3R      , R(rd) = (unsigned)src1 >> src2);
  INSTPAT("0000000000 0110000????? ????? ?????"   , SRA.W    , 3R      , R(rd) = (int32_t)src1 >> (src2 & 0x1F));

  INSTPAT("0000000001 0000001????? ????? ?????"   , SLLI.W   , 2RUI5    ,R(rd) = (unsigned)src1 << (unsigned)imm);
  INSTPAT("0000000001 0001001????? ????? ?????"   , SRLI.W   , 2RUI5    ,R(rd) = (unsigned)src1 >> (unsigned)imm);
  INSTPAT("0000000001 0010001????? ????? ?????"   , SRAI.W   , 2RUI5    ,R(rd) = src1 >> (unsigned)imm);

  INSTPAT("0000000000 0101010????? ????? ?????"   , or       , 3R    , R(rd) = src1 | src2);
  INSTPAT("0000000000 0101011????? ????? ?????"   , xor      , 3R    , R(rd) = src1 ^ src2);

  INSTPAT("0000000000 0100100????? ????? ?????"   , SLT      , 3R    , R(rd) = (signed)src1 < (signed)src2 ? 1 : 0;);
  INSTPAT("0000000000 0100101????? ????? ?????"   , SLTU     , 3R    , R(rd) = (unsigned)src1 < (unsigned)src2 ? 1 : 0;);

  INSTPAT("0000000000 0101001????? ????? ?????"   , AND      , 3R    , R(rd) = src1 & src2);
  INSTPAT("0000000000 0101000????? ????? ?????"   , NOR      , 3R    , R(rd) = ~(src1 | src2));


  INSTPAT("010100???? ???????????? ????? ?????"   , B        , I26   , s->dnpc=s->pc+ SEXT(imm<<2,GRLEN));

  INSTPAT("010011???? ???????????? ????? ?????"   , jirl     , 2RI16 , R(rd) = s->pc + 4; s->pc = src1 + imm);
  INSTPAT("010110???? ???????????? ????? ?????"   , BEQ      , 2RI16 , if(R(rd) == src1) s->pc += imm);
  INSTPAT("010111???? ???????????? ????? ?????"   , BNE      , 2RI16 , if(R(rd) != src1) s->pc += imm);
  INSTPAT("011000???? ???????????? ????? ?????"   , BLT      , 2RI16 , if(R(rd) > src1)  s->pc += imm);
  INSTPAT("011001???? ???????????? ????? ?????"   , BGE      , 2RI16 , if(R(rd) <= src1)  s->pc += imm);
  INSTPAT("011010???? ???????????? ????? ?????"   , BLTU     , 2RI16 , if((uint32_t)R(rd) > (uint32_t)src1) s->pc += imm);
  INSTPAT("011011???? ???????????? ????? ?????"   , BGEU     , 2RI16 , if((uint32_t)R(rd) <= (uint32_t)src1) s->pc += imm);

  INSTPAT("0011100000 1110100????? ????? ?????"   , DBAR     , LANZAN , imm = 0);
  INSTPAT("0011100000 1110101????? ????? ?????"   , IBAR     , LANZAN , imm = 0);

  INSTPAT("0000 0000 0010 10100 ????? ????? ?????", break    , N     , NEMUTRAP(s->pc, R(4))); // R(4) is $a0
  INSTPAT("????????????????? ????? ????? ?????"   , inv      , N     , INV(s->pc));
  INSTPAT_END();

  R(0) = 0; // reset $zero to 0

  return 0;
}

int isa_exec_once(Decode *s) {
  s->isa.inst.val = inst_fetch(&s->snpc, 4);
  return decode_exec(s);
}


