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

#include "sdb.h"
#define NR_WP 32

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */
WP* new_wp(){
   for(WP* p = free_; p->next != NULL ; p = p->next){
      if(p->use_flag == false){
         p->use_flag = true;
         if(head == NULL){
            head = p;
         }
         return p;
      }
   }
   printf("No unuse point\n");
   assert(0);
   return NULL;
}

void free_wp(int no){
  WP* p = head;
  if(head == NULL){
     printf("watchpoint list is empty\n");
     assert(0);
  }
  else if(p->NO == no){
    head = head->next;
    p->old_value = 0;
    p->use_flag = false;
    p->next = free_;
    free_ = p;
    printf("delete watchpoint No:%d\n", no);
    return ;
  }
  else{
      WP *q = head;
      p = p -> next;
      while(p!=NULL){
          if(p->NO == no){
             q->next = p->next;
             p->old_value = 0;
             p->use_flag = false;
             p->next = free_;
             printf("delete watchpoint No:%d\n", no);
             return;
          }else{
             p = p->next;
             q = q->next;
          }
      }
      printf("watchpoint No:%d do not exist \n",no);
      return;
  }
}

void watchpoint_show(){
  bool flag = true;
  for(int i = 0;i<NR_WP;i++){
    if(wp_pool[i].use_flag){
        printf("Watchpoint.No: %d, expr = %s, old_value = 0x%08X, new_value = 0x%08X\n",
        wp_pool[i].NO,wp_pool[i].expr,wp_pool[i].old_value,wp_pool[i].new_value);
        flag = false;
    }
  }
  if(flag) printf("No watchpoint now.\n");
}

void delete_watchpoint(int no){
    for(int i = 0;i<NR_WP;i++){
       if(wp_pool[i].NO == no){
          free_wp(no);
          return ; 
       }
    }
}

void create_watchpoint(char* args){
  WP* p = new_wp();
  strcpy(p -> expr,args);
  bool success = false;
  int tmp = expr(p -> expr,&success);
  if(success) {p -> old_value = tmp; p -> new_value = tmp;}
  else {
    printf("Get expr value error when create watchpoint\n");
  }
  printf("Create watchpoint NO.%d success\n", p->NO);

}

/*scan watchpoint*/
void check_watchpoint(){
  bool success = false;
  WP *cur = head;
  while(cur!=NULL){
    int temp = expr(cur->expr,&success);
    //if(!success){printf("The expression of watchpoint No%d is invalid\n",cur->NO);}
    if(temp != cur->old_value){
      cur->new_value = temp;
      printf(" old_value:0x%08d\n new_value:0x%08X\n",cur->old_value,cur->new_value);
      nemu_state.state = NEMU_STOP;
      cur->old_value = temp;
    }
    
    cur = cur->next;
  }

}

void breakpoint(){
  bool success = false;
  WP *cur = head;
  while(cur!=NULL){
    int temp = expr(cur->expr,&success);
    if(temp == cur->old_value){
      cur->new_value = temp;
      printf(" old_value:0x%08d\n new_value:0x%08X\n",cur->old_value,cur->new_value);
      nemu_state.state = NEMU_STOP;
      cur->old_value = temp;
    }
  }
}