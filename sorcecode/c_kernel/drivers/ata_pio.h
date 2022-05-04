#ifndef ATA_PIO_H
  #define ATA_PIO_H

  typedef struct{
    uint16 codeDevice;
    uint16 flags;     // 0000-0000 = Not valid

  }__attribute__((packed))
  DeviceAta;


  #define ATA_PRIMARY 0x1f0
  #define ATA_SECONDARY 0x170

  #define DCR_PRIMARY 0x3F6
  #define DCR_SECONDARY 0x3f6

  uint8 identify_device(DeviceAta*);

  extern uint8 *BUFFER_HD;

  extern void ata_lba_read(void);
  extern void ata_lba_write(void);
  void ataInit();
#endif
