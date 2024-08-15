#include <NDL.h>
#include <assert.h>
#include <sdl-audio.h>
#include <sdl-timer.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SdlAudioCallback_t audio_callback;

int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained) {
  if (desired == NULL) {
    return -1;
  }
  // 传递desired结构体中的频率、通道数和样本数来打开音频设备。
  NDL_OpenAudio(desired->freq, desired->channels, desired->samples);
  if (obtained != NULL) {
    memcpy(obtained, desired, sizeof(SDL_AudioSpec));
  }
  // printf("SDL_OpenAudio\n");
  // 如果格式是 AUDIO_S16，则样本大小为 uint16_t，否则为 uint8_t。
  const size_t sample_size = desired->format == AUDIO_S16 ? sizeof(uint16_t) : sizeof(uint8_t);
  audio_callback = (SdlAudioCallback_t){
      .callback = desired->callback,
      .userdata = desired->userdata,
      .last_called = 0,
      // 回调函数的调用间隔(单位:毫秒)计算方法是每秒钟的样本数除以频率，再除以1000，得到毫秒数。
      .interval = desired->samples / desired->freq / 1000,
      // 音频缓冲区的大小
      .buf_size = desired->samples * desired->channels * sample_size,
      .buf = NULL,
      .valid = true,
      .paused = false,
      .locked = false,
  };

  audio_callback.buf = (uint8_t *)malloc(sizeof(uint8_t) * audio_callback.buf_size);
  if (audio_callback.buf == NULL) {
    return -1;
  }

  return 0;
}

void SDL_CloseAudio(void) {
  NDL_CloseAudio();
  audio_callback.valid = false;
  if (audio_callback.buf != NULL) {
    free(audio_callback.buf);
    audio_callback.buf = NULL;
  }
}

void SDL_PauseAudio(int pause_on) {
  audio_callback.paused = !!pause_on;
}

#define MIN(x_, y_) ((x_) <= (y_) ? (x_) : (y_))
#define MAX(x_, y_) ((x_) >= (y_) ? (x_) : (y_))

void SDL_MixAudio(uint8_t *dst, uint8_t *src, uint32_t len, int volume) {
}

SDL_AudioSpec *
SDL_LoadWAV(const char *file, SDL_AudioSpec *spec, uint8_t **audio_buf, uint32_t *audio_len) {
  return NULL;
}

void SDL_FreeWAV(uint8_t *audio_buf) {
  free(audio_buf);
}

void SDL_LockAudio(void) {
  audio_callback.locked = true;
}

void SDL_UnlockAudio(void) {
  audio_callback.locked = false;
}

void sdl_schedule_audio_callback(void) {
  if (!audio_callback.valid || audio_callback.paused || audio_callback.locked) {
    return;
  }
  if (NDL_GetTicks() - audio_callback.last_called < audio_callback.interval) {
    return;
  }

  const size_t size = audio_callback.buf_size;
  memset(audio_callback.buf, 0, sizeof(uint8_t) * size);

  audio_callback.callback(audio_callback.userdata, audio_callback.buf, size);
  NDL_PlayAudio(audio_callback.buf, size);
  audio_callback.last_called = NDL_GetTicks();
}