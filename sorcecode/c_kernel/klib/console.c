#include "../header/gfx.h"
#include "../header/Alloc.h"
#include "../header/iopipe.h"
#include "../lib/String.h"

char getChar();

void paintConsole(Console *c,char *linha){
  //uint64 len=strlen(linha);
  uint64 x=0;
  while(linha[x]!='\0'){
    switch(linha[x]){
      case '\n':
        c->atualX=0;
        c->atualY+=1;
        if(c->atualY>=c->tamY)c->atualY=0;
        break;
      case '\t':
        c->atualX+=3;
        if(c->atualX>=c->tamX){
          c->atualX=0;
          c->atualY+=1;
          if(c->atualY>=c->tamY)c->atualY=0;
        }
        break;
      case '\b':
        if(c->atualX>0)c->atualX-=1;
        else{
          c->atualX=c->tamX-1;
          if(c->atualY>0)c->atualY-=1;
          else c->atualY=c->tamY-1;
        }
        kprintChar(c->x+c->atualX,c->y+c->atualY,' ',c->cor);
        break;
      default:
        kprintChar(c->x+c->atualX,c->y+c->atualY,linha[x],c->cor);
        c->atualX++;
        if(c->atualX>=c->tamX){
          c->atualX=0;
          c->atualY+=1;
          if(c->atualY>=c->tamY)c->atualY=0;
        }
    }
    x++;
  }
  if(c->cursor)update_cursor(c->x+c->atualX,c->y+c->atualY);
}


Console* createConsole(uint16 x,uint16 y, uint16 tamX, uint16 tamY,boolean cursor){
    Console *c=(Console*)aloca(sizeof(Console));
    c->out.funcOut=paintConsole;
    c->x=x;
    c->y=y;
    c->atualX=0;
    c->atualY=0;
    c->cor=0x0f;
    c->cursor=cursor;
    c->tamX=tamX;
    c->tamY=tamY;
    return c;
}
