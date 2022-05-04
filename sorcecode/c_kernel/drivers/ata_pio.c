#include "ata_pio.h"

uint8 flagsAtaExist=0;

DeviceAta ata;

void ataInit(){
  ata.codeDevice=ATA_PRIMARY;
  BUFFER_HD=aloca(512);
  uint8 p=identify_device(&ata);
  if(ata.flags){
    kprintStr(0,20,"Ata Primario Mestre OK!",0x1F);
  }else {
    kprintStr(0,20,"Ata Primario Mestre nao foi encontrado!",0x1F);
  }
  kprintnum(0,10,p);
}
