#ifndef IOPIPE_H
    #define IOPIPE_H

    #include "asmFunctions.h"

    typedef struct{
    	void* funcOut;	//funcOut(const char *);
    }output_stream;

    typedef struct{
      output_stream out;
      uint16 x,y;
      uint16 tamX,tamY;
      uint16 cor;
      uint16 atualX,atualY;
      boolean cursor;
    }Console;


    void disable_cursor();
    void enable_cursor ( uint8 cursor_start ,  uint8 cursor_end );
    void update_cursor(int x, int y);
    uint16 get_cursor_position();

#endif
