#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {

  uint32_t key_check = inl(KBD_ADDR); //读取键盘设备寄存器

  if(key_check != KEYDOWN_MASK){
    kbd->keydown = true;
    kbd->keycode = key_check & ~KEYDOWN_MASK;
  }else{
    kbd->keydown = false;
    kbd->keycode = AM_KEY_NONE;
  }
  
}
