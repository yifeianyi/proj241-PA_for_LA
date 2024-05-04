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
#include <cpu/difftest.h>
#include "../local-include/reg.h"
void diff_compare_regsdisplay(CPU_state *ref_r,int idx){
  for (int i = 0; i < 32; i++)
  {
    if(i%3==0 && i!=0)printf("\n");
    if(i==idx){
      printf("\033[31m%4s:  dut = "FMT_PADDR"\t  ref = "FMT_PADDR"\033[37m \033[0m\t", reg_name(i), gpr(i),ref_r->gpr[i]);
    }
    else{
      printf("%4s:  dut = "FMT_PADDR"\t  ref = "FMT_PADDR"\t", reg_name(i), gpr(i),ref_r->gpr[i]);
    }
  }
  printf("\n");
}
bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
    for(int i=0;i<32;i++){
    if(ref_r->gpr[i] != gpr(i))
    {
      Log("reg is error!!!");
      printf("PC:\n\tdut:"FMT_PADDR" \n\tref:"FMT_PADDR"\n",pc,ref_r->pc);
      
      diff_compare_regsdisplay(ref_r,i);
      return false;
    }
  }
  
  return true;
}

void isa_difftest_attach() {
}
