#include <common.h>

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
  //return 0;
}

size_t events_read(void *buf, size_t offset, size_t len) {
   AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
  if (ev.keycode == AM_KEY_NONE)
    return 0;
  size_t ret = snprintf(buf, len, "%s %s\n", ev.keydown?"kd":"ku", keyname[ev.keycode]);
  return ret;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) {
  char temp_buf[30];//作为容器记录VGA的长度和宽度
  memset(temp_buf, 0, sizeof(temp_buf));//初始化容器
  AM_GPU_CONFIG_T ev = io_read(AM_GPU_CONFIG);
  int width = ev.width;
  int height = ev.height;
  int ret = sprintf(temp_buf, "WIDTH : %d\nHEIGHT : %d", width, height);
  // ret -> exclude terminating null character
  if (ret >= len) {
      strncpy(buf, temp_buf, len - 1);
      ret = len;
  } else {
      strncpy(buf, temp_buf, ret);
  }
  return ret;
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  AM_GPU_CONFIG_T ev = io_read(AM_GPU_CONFIG);
  int width = ev.width;
 
  offset /= 4;
  len /= 4;
 
  int y = offset / width;
  int x = offset - y * width;
  io_write(AM_GPU_FBDRAW, x, y, (void *)buf, len, 300, true);
  return len;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
