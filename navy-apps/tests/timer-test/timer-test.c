#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <NDL.h>

int main(){
    NDL_Init(0);
    uint32_t sec = 0;
    uint32_t usec = 0;
    printf("===== In timer-test ======\n");
    while(1) {
    while(usec/100000 < sec){
      usec = NDL_GetTicks();
      printf("usec is %d\n",usec);
    }
    usec = NDL_GetTicks();
    printf("some setence~~\n----------------\n");
    sec += 5;
  }
    return 0;
}
