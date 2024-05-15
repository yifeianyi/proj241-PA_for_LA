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

#ifndef __ISA_LOONGARCH32R_H__
#define __ISA_LOONGARCH32R_H__

#include <common.h>

enum{
  CSR_CRMD, CSR_PRMD,
  CSR_ESTAT = 0x5,
  CSR_ERA,
  CSR_EENTRY = 0xc,
};

typedef struct {
  word_t gpr[32];
  vaddr_t pc;
  word_t era,estat,prmd,eentry;
} loongarch32r_CPU_state;

// decode
typedef struct {
  union {
    uint32_t val;
  } inst;
} loongarch32r_ISADecodeInfo;

#define isa_mmu_check(vaddr, len, type) (MMU_DIRECT)
enum {
  TYPE_2R   ,   TYPE_3R    ,        TYPE_4R,
  TYPE_2RI8 ,   TYPE_2RI12 ,        TYPE_2RI14,  TYPE_CSRXCHG,
  TYPE_2RI16,   TYPE_1RI20 ,        TYPE_1RI21,  TYPE_CSR,
  TYPE_N,       TYPE_S     ,        TYPE_I26,    TYPE_LANZAN, 
  TYPE_2RUI5,   TYPE_CPROP,   TYPE_IDLE,   TYPE_INVTLB,
  TYPE_2RUI12,  TYPE_PRELD,   NONE_TYPE,   TYPE_1R,// none
};


#endif


