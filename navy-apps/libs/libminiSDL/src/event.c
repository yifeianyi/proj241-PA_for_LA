#include <NDL.h>
#include <SDL.h>
#include <string.h>
#include <stdlib.h>
#define keyname(k) #k,

static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
  return 0;
}
static uint8_t key_state[sizeof(keyname) / sizeof(keyname[0])] = {0};
int SDL_PollEvent(SDL_Event *ev) {
  SDL_Init(3);
  unsigned buf_size = 32;
  char *buf = (char *)malloc(buf_size * sizeof(char));
  int ndl_flag = NDL_PollEvent(buf, buf_size);
  // printf("NDL_PollEvent(buf, buf_size) = %d\n", ndl_flag);
  if (ndl_flag == 5 || ndl_flag == 9 || ndl_flag == 10 || ndl_flag == 13) {
    if (strncmp(buf, "kd", 2) == 0) {
        ev->key.type = SDL_KEYDOWN;
    } else {
        ev->key.type = SDL_KEYUP;
    }
    // printf("buf = %s\n",buf);
    // printf("ev->key.type: %d\n", ev->key.type);
    // int flag = 0;
    for (unsigned i = 0; i < sizeof(keyname) / sizeof(keyname[0]); ++i) {
      // printf("i = %d\n",i);
      if (strncmp(buf + 3, keyname[i], strlen(buf) - 4) == 0
            && strlen(keyname[i]) == strlen(buf) - 4) {
        // flag = 1;
        // printf("ev->key.keysym.sym: %d\n", i);
        ev->key.keysym.sym = i;
        break;
      }
    }
    
    free(buf);
    return 1;
  } else {
      return 0;
  }
}

int SDL_WaitEvent(SDL_Event *event) {
  SDL_Init(3);
  // printf("[SDL_WaitEvent] please push key \n");
  while (SDL_PollEvent(event) == 0);
  return 1;
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  if (numkeys != NULL)
    *numkeys = sizeof(key_state) / sizeof(uint8_t);
  return key_state;
}
