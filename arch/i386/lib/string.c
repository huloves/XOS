#include <asm/types.h>
#include <linux/debug.h>

void memset(void* dst_, uint8_t value, uint32_t size) {
   if(dst_ == NULL) {
       BUG();
   }
   uint8_t* dst = (uint8_t*)dst_;
   while (size-- > 0)
      *dst++ = value;
}

void memcpy(void* dst_, const void* src_, uint32_t size) {
   if(dst_ == NULL || src_ == NULL) {
       BUG();
   }
   uint8_t* dst = dst_;
   const uint8_t* src = src_;
   while (size-- > 0)
      *dst++ = *src++;
}

int memcmp(const void* a_, const void* b_, uint32_t size) {
   const char* a = a_;
   const char* b = b_;
   if(a == NULL || b == NULL) {
       BUG();
   }
   while (size-- > 0) {
      if(*a != *b) {
	 return *a > *b ? 1 : -1; 
      }
      a++;
      b++;
   }
   return 0;
}

char* strcpy(char* dst_, const char* src_) {
   if(dst_ == NULL || src_ == NULL) {
       BUG();
   }
   char* r = dst_;
   while((*dst_++ = *src_++));
   return r;
}

uint32_t strlen(const char* str) {
   if(str == NULL) {
       BUG();
   }
   const char* p = str;
   while(*p++);
   return (p - str - 1);
}

int8_t strcmp (const char* a, const char* b) {
   if(a == NULL || b == NULL) {
       BUG();
   }
   while (*a != 0 && *a == *b) {
      a++;
      b++;
   }
   return *a < *b ? -1 : *a > *b;
}

char* strchr(const char* str, const uint8_t ch) {
   if(str == NULL) {
       BUG();
   }
   while (*str != 0) {
      if (*str == ch) {
          return (char*)str;
      }
      str++;
   }
   return NULL;
}


char* strrchr(const char* str, const uint8_t ch) {
   if(str == NULL) {
       BUG();
   }
   const char* last_char = NULL;

   while (*str != 0) {
      if (*str == ch) {
	 last_char = str;
      }
      str++;
   }
   return (char*)last_char;
}

char* strcat(char* dst_, const char* src_) {
   if(dst_ == NULL || src_ == NULL) {
       BUG();
   }
   char* str = dst_;
   while (*str++);
   --str;
   while((*str++ = *src_++));
   return dst_;
}

uint32_t strchrs(const char* str, uint8_t ch) {
   if(str == NULL) {
       BUG();
   }
   uint32_t ch_cnt = 0;
   const char* p = str;
   while(*p != 0) {
      if (*p == ch) {
	 ch_cnt++;
      }
      p++;
   }
   return ch_cnt;
}

char* itoa(int num,char* dst,int radix)
{
    char* p = dst;
    unsigned digval;
    char* firstDig = dst;
    char temp;

    if(num < 0)
    {
        *p++ = '-';
        firstDig++;
        num = 0 - num;
    }

    while(num > 0)
    {
        digval = num%radix;
        num = num/radix;

        if(digval > 9)
        {
            *p++ = (char)(digval - 10 + 'a');
        }
        else
        {
            *p++ = (char)digval + '0';
        }
    }

    *p-- = '\0';

    while(firstDig < p)
    {
        temp = *firstDig;
        *firstDig = *p;
        *p = temp;

        firstDig++;
        p--;
    }

    return dst;
}

char* uitoa(unsigned int num,char* dst,int radix)
{
    char* p = dst;
    unsigned digval;
    char* firstDig = dst;
    char temp;

    while(num > 0)
    {
        digval = num%radix;
        num = num/radix;

        if(digval > 9)
        {
            *p++ = (char)(digval - 10 + 'a');
        }
        else
        {
            *p++ = (char)digval + '0';
        }
    }
    if (p == dst)
        *p++ = '0';

    *p-- = '\0';

    while(firstDig < p)
    {
        temp = *firstDig;
        *firstDig = *p;
        *p = temp;

        firstDig++;
        p--;
    }

    return dst;
}

uint32_t strnlen(const char* str, uint32_t max)
{
    if(str == NULL) {
        BUG();
    }
    uint32_t len = strlen(str);
    return len >= max ? max : len;
}

char * strstr(const char * s1,const char * s2)
{
    int l1, l2;

    l2 = strlen(s2);
    if (!l2)
        return (char *) s1;
    l1 = strlen(s1);
    while (l1 >= l2) {
        l1--;
        if (!memcmp(s1,s2,l2))
            return (char *) s1;
        s1++;
    }
    return NULL;
}

int strncmp(const char * cs,const char * ct,size_t count)
{
    register signed char __res = 0;

    while (count) {
        if ((__res = *cs - *ct++) != 0 || !*cs++)
            break;
        count--;
    }

    return __res;
}