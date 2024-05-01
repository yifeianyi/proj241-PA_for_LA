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

#ifndef __LOONGARCH32R_REG_H__
#define __LOONGARCH32R_REG_H__

#include <common.h>

static inline int check_reg_idx(int idx) {
  IFDEF(CONFIG_RT_CHECK, assert(idx >= 0 && idx < 32));
  return idx;
}

#define gpr(idx) cpu.gpr[check_reg_idx(idx)]

static inline const char* reg_name(int idx) {
  extern const char* regs[];
  return regs[check_reg_idx(idx)];
}

#define R(i) gpr(i)
#define Mr vaddr_read
#define Mw vaddr_write
//===================================================================================
enum {
  TYPE_2R   ,   TYPE_3R    ,    TYPE_4R,
  TYPE_2RI8 ,   TYPE_2RI12 ,    TYPE_2RI14,
  TYPE_2RI16,   TYPE_1RI20 ,    TYPE_1RI21,  TYPE_2RI14_1,
  TYPE_N,       TYPE_S     ,    TYPE_I26,    TYPE_LANZAN, 
  TYPE_3R_U,    TYPE_2RI12_CP,  TYPE_ID,     TYPE_IN,
  TYPE_2RI12_U, TYPE_2RI12_PR,  NONE_TYPE,   TYPE_2R_1,// none
};

#define src1R()  do { *src1 = R(rj); } while (0)
#define simm12() do { *imm = SEXT(BITS(i, 21, 10), 12); } while (0)
#define simm20() do { *imm = SEXT(BITS(i, 24, 5), 20) << 12; } while(0)

#endif
