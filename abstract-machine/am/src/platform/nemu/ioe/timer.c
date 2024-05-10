#include <am.h>
#include <nemu.h>

void __am_timer_init() {
  outl(RTC_ADDR,0);       // 向64位的时钟数据寄存器地址的低32位写入0
  outl(RTC_ADDR+4,0);     // 向64位的时钟数据寄存器地址的高32位写入0
}


void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uptime->us = (uint64_t)inl(RTC_ADDR+4);
  uptime->us = (uptime->us)<<32; 
  uptime->us = uptime->us + (uint64_t)inl(RTC_ADDR);
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour   = 0;
  rtc->day    = 0;
  rtc->month  = 0;
  rtc->year   = 1900;
}
