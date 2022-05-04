#include <stdio.h>
#include <stdlib.h>


void main(){
  FILE *f,*global_main;
  int x=1,tam=0;
  char buffer[256];
  while(x){
    sprintf(buffer,"../public/main%d.c",x);
    f=fopen(buffer,"r");
    if(!f){
      tam=x;
      x=0;
    }else{
      x++;
      fclose(f);
    }
  }
  global_main=fopen("../public/global_main.c","w");
  for(x=1;x<tam;x++){
    fprintf(global_main,"#include \"main%d.c\"\r\n",x);
  }
  fprintf(global_main,"int prepare_programs(void** arr){\r\n");
  for(x=1;x<tam;x++){
    fprintf(global_main,"\tarr[%d]=(void*)main%d;\r\n",x-1,x);
  }
  fprintf(global_main,"\treturn %d;\r\n}",x-1);
  fclose(global_main);
}
