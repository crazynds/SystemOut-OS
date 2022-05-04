#include "../header/Tables.h"
#include "../header/Alloc.h"

typedef struct {
  uint32 reserv1;
  uint64 *rsp0;
  uint64 *rsp1;
  uint64 *rsp2;
  uint64 reserv2;
  uint64 *ist1;
  uint64 *ist2;
  uint64 *ist3;
  uint64 *ist4;
  uint64 *ist5;
  uint64 *ist6;
  uint64 *ist7;
  uint64 reserv3;
  uint16 reserv4;
  uint16 IOPB;
}__attribute__((packed)) Tss;


Tss tast_manager;
uint64 stack_master[4096];

extern void tss_flush(uint32 tss);


void TSSInit(){
  tast_manager.rsp0=(uint64*)(&stack_master+4095*8);
  uint32 val=addEntryGDT(&tast_manager,sizeof(Tss),0xFA,0xA);
  tss_flush(0x28);
}
