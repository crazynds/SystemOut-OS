#ifndef IDT_H
  #define IDT_H


  void idtInit();
  void GDTInit();
  uint32 addEntryGDT(uint32 base, uint32 limit,uint16 acess,uint8 flags);

#endif
