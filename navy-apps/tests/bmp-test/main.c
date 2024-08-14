#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <NDL.h>
#include <BMP.h>

int main() {
  NDL_Init(0);
  int w, h;
  void *bmp = BMP_Load("/share/pictures/projectn.bmp", &w, &h);
  assert(bmp);
  printf("bmp-w:%d, bmp-h:%d\n",w,h);
  NDL_OpenCanvas(&w, &h);

  printf("before Draw\n");
  NDL_DrawRect(bmp, 0, 0, w, h);
  printf("after Draw\n");
  free(bmp);
  NDL_Quit();
  printf("Test ends! Spinning...\n");
  while (1);
  return 0;
}
