#include "../header/iopipe.h"
#include "../header/Variaveis.h"



void disable_cursor(){
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}
void enable_cursor ( uint8 cursor_start ,  uint8 cursor_end ) {
	outb ( 0x3D4 ,  0x0A ) ;
	outb ( 0x3D5 ,  ( inb ( 0x3D5 )  &  0xC0 )  | cursor_start ) ;

	outb ( 0x3D4 ,  0x0B ) ;
	outb ( 0x3D5 ,  ( inb ( 0x3D5 )  &  0xE0 )  | cursor_end ) ;
}
void update_cursor(int x, int y){
	uint16 pos = y * 80 + x;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8) ((pos >> 8) & 0xFF));
}

uint16 get_cursor_position(){
    uint16 pos = 0;
    outb(0x3D4, 0x0F);
    pos |= inb(0x3D5);
    outb(0x3D4, 0x0E);
    pos |= ((uint16)inb(0x3D5)) << 8;
    return pos;
}
