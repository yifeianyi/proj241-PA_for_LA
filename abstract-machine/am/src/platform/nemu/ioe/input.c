#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {  
  uint32_t keycheck = inl(KBD_ADDR);
  if(keycheck & KEYDOWN_MASK){
    kbd->keydown = true;
    kbd->keycode = keycheck & ~KEYDOWN_MASK;
  }else{
    kbd->keydown = false;
    kbd->keycode = AM_KEY_NONE;
  }
  
}
