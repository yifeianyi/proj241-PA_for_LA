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

#include <isa.h>

word_t isa_raise_intr(word_t NO, vaddr_t epc, bool flag_sys) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * Then return the address of the interrupt/exception vector.
   */
  cpu.era = epc;
  cpu.estat = NO<<16;
  word_t Ecode,EsubCode;
  if(NO==-1){
    Ecode = -1;
    EsubCode = -1;
  } 
  else if(flag_sys){
    Ecode = 0xB;
    EsubCode = 0;
  }
  else{
    Ecode = BITS(NO,5,0);
    EsubCode = BITS(NO,15,6);
  }
  cpu.estat = BITS(EsubCode,7,0)<<22 | BITS(Ecode,5,0)<<16;
#ifdef CONFIG_ETRACE
  printf("test testtest.\n");
#endif
  return cpu.eentry;
}
word_t isa_query_intr() {

  return cpu.era;
}
