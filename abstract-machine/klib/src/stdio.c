#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

static void reverse(char *s,int len){
  char *end = s + len -1;
  char tmp;
  while(s < end){
     tmp = *s;
     *s = *end;
     *end = tmp;
     s++;
     end--;
  }
}

static int itoa(int n,char* s,int base)
{
  assert(base <= 16);
  int i = 0,sign = n,bit;
  if(sign < 0) n = -n;
  do {
   /*
    bit = n % base;
    if(bit>=0) s[i++] = 'a'+bit -10;
    else s[i++] = '0' + bit;
   */
   bit = n % base;
   s[i++] = '0' + bit;
  } while((n/=base)>0);
  if(sign < 0) s[i++] = '-';
  s[i] = '\0';
  reverse(s,i);

  return i;
}

static char* print_num(char* out, uint64_t num, uint64_t base, int len, char ch){
  if(num == 0) {
    for(int i = 0; i < len; i++) *out++ = ch;
    return out;
  }
  out = print_num(out, num / base, base, len - 1, ch);
  *(out++) = "0123456789abcdef"[num%base];
  return out;
}

static char* _fill(char* out, char ch, int len){
  for(int i = 0; i < len; i ++) *out++ = ch;
  return out;
}
int vsprintf(char *out, const char *fmt, va_list ap) {
  int idx = 0;
  while(fmt[idx] != 0){
    if(fmt[idx] != '%'){
      *(out) = fmt[idx ++];
      out ++;
      continue;
    }
    idx++;
    int val = 0;
    uint32_t uval = 0;
    uint64_t ulval = 0;
    int64_t lval = 0;
    char *tem_s;
    char tem_c;
    int width = 0;
    int flag = 0;
    int len = 0;
    if(fmt[idx] == '0') { flag = 1; idx ++;}
    else if(fmt[idx] == ' ') {flag = 2; idx ++;}
    while(fmt[idx] >= '0' && fmt[idx] <= '9'){
      width = width * 10 + fmt[idx] - '0';
      idx ++;
    }
    if(fmt[idx] == 'l') {
      flag |= 4;
      idx ++;
    }
    switch(fmt[idx++]){
      case 's': 
          tem_s = va_arg(ap, char*);
          len = strlen(tem_s);
          if(flag == 1) out = _fill(out, '0', width - len > 0? width-len:0);
          else if(flag == 2) out = _fill(out, ' ', width - len > 0? width-len:0);
          strcpy(out, tem_s);
          out += strlen(tem_s);
          break;
      case 'd':
          if(flag & 4) lval = va_arg(ap, int64_t);
          else val = va_arg(ap, int);
          if(val < 0 || lval < 0) {
            *(out++) = '-';
            val = -val;
          }
          if(val == 0 && lval == 0) {
            *(out++) = '0';
            for(int i = 1; i < width; i++) *out++ = '0';
          }
          else {
            if(flag & 1) out = print_num(out, flag & 4? lval:val, 10, len, '0');
            else if(flag & 2) out = print_num(out, flag & 4? lval:val, 10, len, ' ');
            else out = print_num(out, flag & 4? lval:val, 10, 0, 0);
          }
          break;
      case 'c':
          tem_c = va_arg(ap, int);
          *out++ = tem_c;
          break;
      case 'p':
      case 'x':
          *out++ = '0';
          *out++ = 'x';
          if(flag & 4) ulval = va_arg(ap, uint64_t);
          else uval = va_arg(ap, uint32_t);
          if(ulval == 0 && uval == 0) {
            *(out++) = '0';
            for(int i = 1; i < width; i++) *out++ = '0';
          }
          else {
            if(flag & 1) out = print_num(out, flag&4? ulval:uval, 16, len, '0');
            else if(flag & 2) out = print_num(out, flag&4? ulval:uval, 16, len, ' ');
            else out = print_num(out, flag&4? ulval:uval, 16, 0, 0);
          }
          break;
      
      default: putch(fmt[idx-1]); putch('\n'); assert(0);
    }  
  }
  *out = 0;
  return 0;
}

int printf(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  char buf[2048];
  memset(buf, 0, sizeof(buf));
  int ret = vsprintf(buf, fmt, args);
  putstr(buf);
  return ret;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list pArgs;
  va_start(pArgs, fmt);
  char *start = out;
  
  for(;*fmt != '\0';++fmt){
    if(*fmt != '%'){
      *out = *fmt;
      ++out;
    }
    else{
     switch(*(++fmt)){
     case '%': *out = *fmt; ++out; break;   
     case 'd': out += itoa(va_arg(pArgs,int),out,10);break;
     case 's':
        {
        char *s = va_arg(pArgs,char*);
        
        strcpy(out,s);
        out += strlen(out);
        
        }
        break;
    case 'c':
        {
        char c = (char)va_arg(pArgs, int);
				*out++ = c;
        }
				break;
     }
     
    }
   }
    *out = '\0';
    va_end(pArgs);

    return out - start;
 // panic("Not implemented");              
}

           
int snprintf(char *out, size_t n, const char *fmt, ...) {
  char str[1024];
  sprintf(str, fmt);
  strncmp(str, out , n);
  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
