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
     case 'd': out += itoa(va_arg(ap,int),out,10);break;
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
  va_list pArgs;
  va_start(pArgs, fmt);
  char *start = out;
  size_t remaining = n;

  for (; *fmt != '\0' && remaining > 0; ++fmt) {
    if (*fmt == '\\') {
      // When we encounter a backslash, check if the next character is an 'n'
      ++fmt; // Move to the next character to check if it's an 'n'
      if (*fmt == 'n') {
        // If there is space, add a newline character, otherwise exit the loop
        if (remaining > 1) {
          *out++ = '\n';
          --remaining;
        } else {
          break;
        }
      } else {
        // If the next character is not an 'n', treat the backslash as a normal character
        if (remaining > 1) {
          *out++ = '\\';
          --remaining;
        }
        // Put the fmt pointer back to the previous character to check it in the next iteration
        fmt--;
      }
    } else if (*fmt != '%') {
      *out++ = *fmt;
      --remaining;
    } else {
      // Handle format specifier
      fmt++; // Move to the specifier character
      switch (*fmt) {
        case '%':
          *out++ = '%';
          --remaining;
          break;
        case 'd': {
          int num = va_arg(pArgs, int);
          int written = snprintf(out, remaining, "%d", num);
          if (written > 0) {
            out += written < remaining ? written : remaining - 1;
            remaining -= written < remaining ? written : remaining - 1;
          }
          break;
        }
        case 's': {
          char *s = va_arg(pArgs, char *);
          int length = strlen(s);
          if (length < remaining) {
            strcpy(out, s);
            out += length;
            remaining -= length;
          } else {
            strncpy(out, s, remaining - 1);
            out[remaining - 1] = '\0'; // Ensure null-termination
            return out + remaining - start - 1; // We've exhausted the space
          }
          break;
        }
        case 'c': {
          char c = (char)va_arg(pArgs, int);
          *out++ = c;
          --remaining;
          break;
        }
        // Other specifiers can be added here
      }
    }
  }
  *out = '\0'; // Null-terminate the string
  va_end(pArgs);

  return out - start;
  //panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
