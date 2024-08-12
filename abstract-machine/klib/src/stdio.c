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

int vsprintf(char *out, const char *fmt, va_list ap) {
  
  char *start = out;
    
  for(;*fmt != '\0';++fmt){
    if(*fmt != '%'){
      *out = *fmt;
      ++out;
    }
    else{
     switch(*(++fmt)){
     case '%': *out = *fmt; ++out; break;
     case 'd':
    {
      int value = va_arg(ap, int);
      char buffer[12]; // Buffer to hold the integer as a string
      itoa(value, buffer, 10);
      int width = 2; // The minimum width
      char fill = ' '; // The fill character

      // Check if the format is %02d or %2d
      if (*(fmt - 1) == '0') {
        fill = '0';
      }

      // Calculate the actual width of the integer
      int actual_width = strlen(buffer);

      // If the actual width is less than the minimum width, we need to add fill characters
      if (actual_width < width) {
          for (int i = 0; i < width - actual_width; ++i) {
            *out++ = fill;
          }
        }

        // Copy the integer to the output
        strcpy(out, buffer);
        out += actual_width;
    }
    break;
     case 's':
        {
        char *s = va_arg(ap,char*);
        strcpy(out,s);
        out += strlen(out);
        }
        break;
     case 'c':
       {
          char c = (char)va_arg(ap,int);
          *out++ = c;
       }
       break;
    case 'p':
       {
           uintptr_t value = (uintptr_t)va_arg(ap, void*);
          *out++ = '0';
          *out++ = 'x';
          for (int i = sizeof(void*) * 2 - 1; i >= 0; --i) {
            // 获得当前十六进制位的字符
            int current_digit = (value >> (4 * i)) & 0xF;
            if (current_digit <= 9) {
                // '0'到'9'之间
                *out++ = '0' + current_digit;
            } else {
                // 'a'到'f'之间
                *out++ = 'a' + (current_digit - 10);
            }
         }
       }
       break;
     }
     
    }
   }
    *out = '\0';

    return out - start;
 // panic("Not implemented");
}

int printf(const char *fmt, ...) {
  char buf[2048];
	va_list ap;
	va_start(ap, fmt);

	int res = vsprintf(buf, fmt, ap);
	putstr(buf);

	va_end(ap);
	return res;
  //panic("Not implemented");
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
