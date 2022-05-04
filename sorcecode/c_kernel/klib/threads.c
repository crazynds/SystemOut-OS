#include "../header/Alloc.h"
#include "../header/gfx.h"
#include "../header/Thread.h"
#include "../lib/stdio.h"


static Thread **thread;
static uint32 tam;

static uint32 next_thread;
static uint64 GLOBAL_OID;

output_stream *consoleGeneral;

void initTasking(){
  thread=(Thread**)maloca(sizeof(Thread*));
  thread[tam]=(Thread*)maloca(sizeof(Thread));
  thread[tam]->OID=GLOBAL_OID++;
  thread[tam]->stack_rbp=0;
  thread[tam]->stack_rsp=0;
  thread[tam]->execution=0;
  thread[tam]->x86_save_512=(void*)((uint64)aloca(544)+16); //Salva todos os registradores x86 FPU, MMX , XMM , MXCSR; Necessita alinhamento de 16 bytes
  consoleGeneral=(output_stream*)createConsole(0,0,40,30,true);
  thread[tam]->output=consoleGeneral;
  NOW_THREAD=thread[tam];
  tam=1;
  next_thread=0;
}

void _kill(uint64 OID){
  uint32 x;
  uint8 bol=false;
  __asm__ ("cli");
  for(x=0;x<tam;x++){
    if(bol && x+1<tam){
      thread[x]=thread[x+1];
    }else if(thread[x]->OID==OID){
      void *aux=(void*)((uint64)thread[x]->x86_save_512-16);
      libera(aux);
      libera(thread[x]);
      thread[x]=thread[x+1];
      bol=true;
    }
  }
  if(bol){
    if(NOW_THREAD->OID==OID){
      NOW_THREAD=0;
    }
    tam--;
    thread=(Thread**)realoca(thread,sizeof(Thread*)*(tam));
  }
  __asm__ ("sti");
}

void killThis(){
  _kill(NOW_THREAD->OID);
  while(1);
}

uint64 createKernelThreadArgs(void* func,void* args,uint64 tamArgs){
  thread=(Thread**)realoca(thread,sizeof(Thread*)*(tam+1));
  thread[tam]=(Thread*)maloca(sizeof(Thread));
  thread[tam]->OID=GLOBAL_OID++;
  thread[tam]->stack_rbp=(uint64)maloca(0x1000)+0x1000;
  uint64 *stack=(uint64*)(thread[tam]->stack_rbp-0x8);
  *stack=tamArgs;
  stack--;
  *stack=(uint64)args;
  thread[tam]->stack_rsp=thread[tam]->stack_rbp-0x10;
  thread[tam]->execution=(uint64)func;
  thread[tam]->x86_save_512=(void*)((uint64)aloca(544)+16);
  thread[tam]->output=consoleGeneral;
  tam++;

  return thread[tam]->OID;
}
uint64 createKernelThread(void* func){
  return createKernelThreadArgs(func,0,0);
}

#define KILL_THREAD (1<<16)
#define DEFAULT_TEMPO_PROCESSAMENTO 2

uint64 vez=0;

uint64 trocarThread(){
  if(tam<=1)return 0;
  uint64 time_slice=DEFAULT_TEMPO_PROCESSAMENTO;
  vez++;
  next_thread++;
  if(next_thread>=tam){
    next_thread=0;
  }
  BEFORE_THREAD=NOW_THREAD;
  NOW_THREAD=thread[next_thread];

  if(BEFORE_THREAD==0)return time_slice+KILL_THREAD;
  else return time_slice;
}
