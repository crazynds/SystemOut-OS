#include "../header/Alloc.h"
#include "String.h"


void strcpy(char *str,char *str2){
    while(*str2 != '\0')*str++ = *str2++;
}
void strcpywl(char *str,char *str2,int tam){
    int i=0;
    if(tam){
        for(i=0;(str2[i] != '\0') && (i < tam);i++)
        {
            str[i] = str2[i];
        }
        str[tam] = '\0';
    }else
    {
        strcpy(str,str2);
    }
}
uint64 strlen(char *str){
    uint64 aux=(uint64)str;
    while(*str++);
    return ((uint64)str-aux);
}
unsigned int findchar(char *str,char c){
    unsigned int i=0;
    while(str[i] != c)
    {
        if(strlen(str) < i){return 0;}
        i++;
    }
    return i;
}
void strclean(char *str){
    while(*str)
    {
        *str = 0;
        str++;
    }
}
int strcmp(const char *cs, const char *ct){
    unsigned char c1, c2;
    int res = 0;
    do {
        c1 = *cs++;
        c2 = *ct++;
        res = c1 - c2;
        if (res)break;
    } while (c1);
    return res;
}
void strcat(char *str,char *str2){
    while(*str++){}
    while(*str2 != '\0')*str++ = *str2++;
}

inline uint64 getNextParam(uint32 *p){
  uint64 val;
  val=64+(8*(*p-4));
  __asm__("mov %%rax , %0;mov %1 , [%%rsp+%%rax]":"=r"(val):"r"(val));
  __asm__("":"=r"(val));
  (*p)++;
  return val;
}

//printf's defines functions
#define FLAG_ZERO_LEFT (1<<0)
#define FLAG_RIGHT_ALIGN (1<<1)
#define FLAG_CAN_NEG (1<<2)
#define PRINT_BUF_LEN 32

//start of printf's functions

extern uint32 sprintf(char *out, char *str,...);


static uint32 _prints(char *out, const char *string,uint32 pc, int width, int flags)
{
	register int padchar = ' ';

	if (width > 0) {
		register int len = 0;
		register const char *ptr;
		for (ptr = string; *ptr; ++ptr) ++len;
		if (len >= width) width = 0;
		else width -= len;
		if (flags & FLAG_ZERO_LEFT) padchar = '0';
	}
	if (!(flags & FLAG_RIGHT_ALIGN)) {
		for ( ; width > 0; --width) {
      out[pc++]=padchar;
		}
	}
	for ( ; *string ; ++string) {
    out[pc++]=*string;
	}
	for ( ; width > 0; --width) {
    out[pc++]=padchar;
	}
  return pc;
}
static uint32 _printi(char *out,  int64 i, uint32 pc, int base, int width, int flags, int letbase)
{
	char print_buf[PRINT_BUF_LEN];
	register char *s;
	register int t, neg = 0;
	register unsigned int u = i;
  uint16 len=0;

	if (i == 0) {
		print_buf[0] = '0';
		print_buf[1] = '\0';
		return _prints (out,print_buf,pc, width, flags);
	}
	if (flags&FLAG_CAN_NEG && base == 10 && i < 0) {
		neg = 1;
		u = -i;
	}
	s = print_buf + PRINT_BUF_LEN-1;
	*s = '\0';
	while (u) {
		t = u % base;
		if( t >= 10 )t += letbase - '0' - 10;
		*--s = t + '0';
		u /= base;
    len++;
	}
	if (neg) {
		if( width && (flags & FLAG_ZERO_LEFT) ) {
      out[pc++]='-';
			--width;
		}
		else {
			*--s = '-';
		}
	}
	return _prints (out, s, pc, width, flags);
}

uint32 _sprintf(char *out, char *str,const void *var_list){
  uint32 pc=0;
  //stack+=0x20;
  for (; *str; ++str) {
    if(*str!='%'){
      out_str:
      out[pc++]=*str;
    }else{
      str++;
      if (*str == '\0'){
        break;
      }
      uint8 n=0;
      uint16 width=0,flags=0;
      do{
        switch(*str){
          case '0': flags   |= FLAG_ZERO_LEFT; str++; n=1;break;
          case '-': flags   |= FLAG_RIGHT_ALIGN; str++; n=1;break;
          default:                                 n=0;break;
        }
      }while(n);
      for ( ; *str >= '0' && *str <= '9'; ++str) {
				width *= 10;
				width += *str - '0';
      }
      switch(*str){
        case 's':{
          char *s = ((char*)var_list);
          pc = _prints (out, s?s:"(null)", pc, width, flags);
          var_list+=8;
        }break;
        case 'u':
          pc = _printi (out, *(uint32*)var_list, pc, 10, width, flags, 'a');
          var_list+=8;
          break;
        case 'l':
          flags|=FLAG_CAN_NEG;
          pc = _printi (out, *(uint64*)var_list, pc, 10, width, flags, 'a');
          var_list+=8;
          break;
        case 'd':
          flags|=FLAG_CAN_NEG;
          pc = _printi (out, *(int32*)var_list, pc, 10, width, flags, 'a');
          var_list+=8;
          break;
        case 'c':
          out[pc++]=*(char*)var_list;
          var_list+=8;
          break;
        case 'o':
          pc = _printi (out, *(uint32*)var_list, pc, 8, width, flags, 'a');
          var_list+=8;
          break;
        case 'x':
          pc = _printi (out, *(uint32*)var_list, pc, 16, width, flags, 'a');
          var_list+=8;
          break;
        case 'X':
          pc = _printi (out, *(uint32*)var_list, pc, 16, width, flags, 'A');
          var_list+=8;
          break;
        case '%':
        default:
          goto out_str;
      }
		}
	}
	out[pc] = '\0';
	return pc;
}
