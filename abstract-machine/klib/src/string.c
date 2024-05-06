#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  if (s == NULL) {
    return 0;
  }
  size_t n = 0;
  while(s[n] != '\0') {
    ++n;
  }
  return n; 
  //panic("Not implemented");
}

char *strcpy(char *dst, const char *src) {
  char *ret = dst;
  while((*dst++ = *src++)){}
  return ret;
  // panic("Not implemented");
}

char *strncpy(char *dst, const char *src, size_t n) {
  if (src == NULL || dst == NULL) {
    return dst;
  }
  char *ans = dst;
  while (*src != '\0' && n != 0) {
    *dst = *src;
    ++dst;
    ++src;
    --n;
  }
  // 灏嗛澶栫殑绌哄瓧绗﹀啓鍏est锛岀洿鍒板啓鍏ヤ簡n涓瓧绗︾殑鎬绘暟銆?
  while (n != 0) {
    *dst = '\0';
    ++dst;
    --n;
  }
  return ans;
  // panic("Not implemented");
}

char *strcat(char *dst, const char *src) {
  char *ret = dst;
  while(*dst != '\0')
  {
      dst++;
  }
  while(*src != '\0')
  {
     *dst++ = *src++;
  }
  *dst = '\0';
  return ret;
  // panic("Not implemented");
}

int strcmp(const char *s1, const char *s2) {
  while(*s1 && (*s1 == *s2)){
    s1++;
    s2++;
  }
  return *(unsigned char*)s1 - *(unsigned char*)s2;
  // panic("Not implemented");
}

int strncmp(const char *s1, const char *s2, size_t n) {
  while(n > 0 && *s1 && (*s1 == *s2))
  {
    s1++;
    s2++;
    n--;
  }
  if(n == 0)
  {
    return 0;
  } 
  return *(unsigned char*)s1 - *(unsigned char*)s2;
  // panic("Not implemented");
}

void *memset(void *s, int c, size_t n) {
  unsigned char *p = (unsigned char *)s;
  for(size_t i = 0;i < n;i++){
      p[i] = (unsigned char)c;
  }
  return s;
  // panic("Not implemented");
}

void *memmove(void *dst, const void *src, size_t n) {
   unsigned char *d = (unsigned char *)dst;
   const unsigned char *s = (const unsigned char *)src;
   
   if(d<=s){
      for(size_t i = 0;i < n;i++){
      d[i] = s[i];
      }
   }else{
      for(size_t i = n;i > 0; i--){
      d[i-1] = s[i-1];
      }
   }
  
   return dst;
  // panic("Not implemented");
}

void *memcpy(void *out, const void *in, size_t n) {
  unsigned char *d = (unsigned char *)out;
  const unsigned char *s = (const unsigned char *)in;
   
  for(size_t i = 0;i < n;i++){
     d[i] = s[i];
  }

  return out;
  // panic("Not implemented");
}

int memcmp(const void *s1, const void *s2, size_t n) {
  const unsigned char *p1 = (const unsigned char *)s1;
  const unsigned char *p2 = (const unsigned char *)s2;

  for(size_t i = 0;i < n; i++){
    if(p1[i] != p2[i]){
        return p1[i] - p2[i];
    }
  }

  return 0;
  //  panic("Not implemented");
}


#endif