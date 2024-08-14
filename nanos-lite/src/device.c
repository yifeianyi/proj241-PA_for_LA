#include <common.h>
#include <fs.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
  for (size_t i = 0; i < len; ++i) putch(*((char *)buf + i));
  return len;
}

size_t events_read(void *buf, size_t offset, size_t len) {
  AM_INPUT_KEYBRD_T t = io_read(AM_INPUT_KEYBRD);
  if (t.keycode == AM_KEY_NONE) {
    *(char*)buf = '\0';
    return 0;
  }
  else{
    int res = sprintf((char *)buf, "%s %s\n", t.keydown ? "kd" : "ku", keyname[t.keycode]);
    printf("%s", buf);
    return res;
  }
}

static uint32_t Canvas_x,Canvas_y;
size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T gpu = io_read(AM_GPU_CONFIG);
  int rlen = sprintf(buf, "WIDTH:%d\nHEIGHT:%d\n", gpu.width, gpu.height);
  // printf("dispinfo_read:\nWIDTH:%d\nHEIGHT:%d\n", gpu.width, gpu.height);
  Canvas_x = gpu.width;
  Canvas_y = gpu.height;
  return rlen;
}

size_t sbctl_read(void *buf, size_t offset, size_t len) {
  AM_AUDIO_CONFIG_T cfg = io_read(AM_AUDIO_CONFIG);
  AM_AUDIO_STATUS_T stat = io_read(AM_AUDIO_STATUS);
  // 总缓冲区大小中减去已处理的字节数来计算剩余的缓冲区大小
  *((uint32_t *)buf) = cfg.bufsize - stat.count;
  return len;
}

#define MIN(x, y) ((x) < (y) ? (x) : (y))

size_t sbctl_write(const void *buf, size_t offset, size_t len) {
  (void)offset;

  struct {
    int freq;
    int channels;
    int samples;
  } __attribute__((packed)) data;
  len = MIN(len, sizeof(data));
  memcpy(&data, buf, len);
  io_write(AM_AUDIO_CTRL, data.freq, data.channels, data.samples);
  return len;
}

size_t sb_write(const void *buf, size_t offset, size_t len) {
  Area wbuf;
  wbuf.start = (void *)buf;
  wbuf.end = (void *)buf + len;
  io_write(AM_AUDIO_PLAY, wbuf);
  return len;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  int x = offset % Canvas_x;  //计算起点
  int y = offset / Canvas_x;
  // Log("[fb write]x:%d y:%d len:%d \n",x,y,len);
  io_write(AM_GPU_FBDRAW,x,y, (uint32_t *)buf, len , 1, true);

  return len;
} 

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
