#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <assert.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

int Init_screen(){
   int buf_size = 1024; 
    char *buf = (char *)malloc(buf_size * sizeof(char));
    assert(buf != NULL); 

    int fd = open("/proc/dispinfo");
    assert(fd >= 0); 

    ssize_t ret = read(fd, buf, buf_size - 1); 
    assert(ret >= 0); 
    buf[ret] = '\0'; 

    assert(close(fd) == 0); 

    
    if (sscanf(buf, "WIDTH : %d\nHEIGHT : %d", &screen_w, &screen_h) != 2) {
        fprintf(stderr, "Failed to read width and height.\n");
        free(buf);
        return; 
    }

    free(buf); 
    printf("%d %d\n",screen_w,screen_h);
    return 0;
}

uint32_t NDL_GetTicks() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int NDL_PollEvent(char *buf, int len) {
  memset(buf, 0, len);
  int fd = open("/dev/events");
  int ret = read(fd, buf, len);
  close(fd);
  return ret == 0 ? 0 : 1;
  //return 0;
}

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
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

  if (*w == 0 && *h == 0) {
    *w = screen_w;
    *h = screen_h;
  }
  
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  int fd = open("/dev/fb");
  //printf("fd = %d\n",fd);
  for (int i = 0; i < 1; i++) { 
    lseek(fd, ((y + i) * screen_w + x) * 4, SEEK_SET);
    write(fd, pixels + i * w, 4*w);
    //printf("%d\n",i);
  }
  //close(fd);
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
  Init_screen();
  return 0;
}

void NDL_Quit() {
}
