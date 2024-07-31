#include <stdio.h>
/*==========new add=========*/
#include <string.h>
/*==========================*/
#include <NDL.h>

int main() {
  NDL_Init(0);
  while (1) {
    char buf[64];
    /*=============new add=============*/
    memset(buf,0,sizeof(buf));
    /*=================================*/
    if(NDL_PollEvent(buf, sizeof(buf))) {
      printf("receive event: %s\n", buf);
    }
  }
  return 0;
}
