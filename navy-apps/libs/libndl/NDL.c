#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#define DEBUG 1
static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;
static int canvas_x = 0, canvas_y = 0;

uint32_t NDL_GetTicks() {
  struct timeval tv;
  assert(gettimeofday(&tv, NULL) == 0);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int NDL_PollEvent(char *buf, int len) {
  int rlen = read(evtdev, buf, len);
  return rlen;
}

void NDL_OpenCanvas(int *w, int *h) {
  if (*w == 0) *w = screen_w;
  if (*h == 0) *h = screen_h;
  canvas_x = *w; canvas_y = *h;

  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }

  printf("[NDL_OpenCanvas]Canvas_x %d Canvas_y %d screen_w %d screen_h %d\n",canvas_x,canvas_y,screen_w,screen_h);
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
    x+=(screen_w -canvas_x )/2;  
    y+=(screen_h -canvas_y )/2;
    for (int i = 0; i < h; ++ i) {   
      lseek(fbdev, ((y + i) * screen_w + x) , SEEK_SET);
      write(fbdev, pixels + i * w, w );    
  }
}

void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }
  evtdev = open("/dev/events", 0);

  //read dispinfo
  int disp = open("/proc/dispinfo",0);
  char disps[1024];
  read(disp,disps,sizeof(disps));
  close(disp);
  sscanf(disps,"%*[A-z] :%d\n%*[A-z] :%d",&screen_w,&screen_h);
  // printf("display:width:%d,  high:%d\n",screen_w,screen_w);

  fbdev = open("/dev/fb",0);
  return 0;
}

void NDL_Quit() {
}
