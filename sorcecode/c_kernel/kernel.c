#include "Head.h"
#include "header/Variaveis.h"
#include "header/Memory.h"

#include "lib/String.h"
#include "lib/stdio.h"

#include "drivers/ata_pio.h"

void terminal(){
  void **arr=(void**)aloca(8*512);
  int progm=prepare_programs(arr);
  arr=(void**)realoca(arr,progm*8);

  while(1){
    printf("\nComecando a execucao dos programas:\n");
    ((void (*)())arr[0])();
    getChar();
    //gets(buff);
  }
}

void kmain(void){
  int y=2;
  clrscr();
  kprintStr(0,y++,"Kernel iniciado em 64bits        ",0x0F);  
  pagingInit(); 
  kprintStr(0,y++,"[PAGINACAO SETADA]               ",0x0F);
  idtInit();
  kprintStr(0,y++,"[IDT SETADA]                     ",0x0F);
  GDTInit();
  kprintStr(0,y++,"[GDT SETADA]                     ",0x0F);

  
  return;

  //alocação para baixo não está funcionando
  memoryManagerStart(allocPage(),0x1000);
  kprintStr(0,y++,"[GERENCIADOR DE MEMORIA INICIADO]",0x0F);
  initTasking();
  kprintStr(0,y++,"[INICIADO MULTI-TAREFA]          ",0x0F);
  TSSInit();
  kprintStr(0,y++,"[TSS SETADO]                     ",0x0F);

  //ataInit();
  //kprintStr(0,9,"[INICIADO DISPOSITIVOS ATA]      ",0x0F);
  //killThis();


  /*
  clrscr();
  for(x=0;x<30;x++){
    kprintChar(80/2,x,'|',0x0F);
  }
  */

  //createKernelThread(terminal);
}
