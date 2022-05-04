#include "../header/Variaveis.h"
#include "../header/Memory.h"

#define MAX_ENTRIES 16

uint16 qtdEntries=0;
struct memory_entry entries[16];


#include "../header/gfx.h"
#include "../lib/String.h"
void printMemoryEntry(){
    char buffer[256];
    clrscr();
    for(int x=0;x<qtdEntries;x++){
        struct memory_entry *aux = &entries[x];
        uint64 qtdPages = (aux->size)/(4*KB);
        uint8 *ptr = aux->start;
        sprintf(buffer,"Entrada[%d]: %X -> %X (%d) %d %d %d %d",x,aux->start,aux->start+aux->size,qtdPages);
        kprintStr(0,x*3,buffer,0x1F);
        for(uint64 y=0;y<48;y++){
            uint64 pos = y/8;
            uint8 bit = y%8;
            if(bit==0){
                sprintf(buffer,"%X",ptr[pos]);
                kprintStr(y,x*3+2,buffer,0x0f);
            }
            if((1<<bit)&ptr[pos]){
                kprintStr(y,x*3+1,"X",0x0F);
            }else{
                kprintStr(y,x*2+1,"-",0x0F);
            }
        }
    }
}

struct memory_entry* getMemoryEntry(uint16 x){
    if(x>qtdEntries)return 0;
    return &entries[x];
}

uint32 addMemoryEntry(uint64 start,uint64 maxSize){
    uint32 leftover=0;
    struct memory_entry *mem= &entries[qtdEntries];
    mem->start=start&0xFFFFFFFFFFFFF000;
    if(mem->start!=start){
        mem->leftoverStart=start-mem->start;
        leftover+=mem->leftoverStart;
        mem->start+=4*KB;
    }else mem->leftoverStart=0;
    mem->size=maxSize&0xFFFFFFFFFFFFF000;
    if(mem->size!=maxSize){
        mem->leftoverSize=maxSize-mem->size;
        leftover+=mem->leftoverSize;
    }else mem->leftoverSize=0;
    if(mem->size > 16*MB){
        uint64 qtdPagesKernel = maxSize/(16*MB);
        uint8 *pages = (uint8*)start;
        if(mem->size%(16*MB)!=0){
            qtdPagesKernel++;
        }
        mem->offset=qtdPagesKernel;
    }else{
        mem->offset=1;
    }
    memzero(mem->start,mem->offset);
    uint8 *ptr = (uint8)mem->start;
    *ptr=1;
    mem->lastAvaliable=0;
    qtdEntries++;
    return mem->size*4*KB;
}

uint64 pagesOcuped(){
    uint64 sum = 0;
    for(int x=0;x<qtdEntries;x++){
        struct memory_entry *aux = &entries[x];
        uint64 qtdPages = (aux->size)/(4*KB);
        uint8 *ptr = aux->start;
        for(uint64 y=aux->lastAvaliable;y<qtdPages;y++){
            uint64 pos = y/8;
            uint8 bit = y%8;
            if((1<<bit)&*ptr){
                sum++;
            }
        }
    }
    return sum;
}


void freePage(void *ptr){
    uint64 val = ((uint64)ptr)&0xFFFFFFFFFFFFF000;
    for(int x=0;x<qtdEntries;x++){
        struct memory_entry *aux = &entries[x];
        if(ptr>=aux->start+aux->offset && ptr<aux->start+aux->size){
            val/=(4*KB);
            uint64 pos = val/8;
            uint8 bit = val%8;
            uint8 *ptr = aux->start;
            ptr[pos]&= ~(1<<bit);
            if(val<aux->lastAvaliable)aux->lastAvaliable=val;
            return;
        }
    }
}
int auxi=0;

void* allocPage(){
    for(int x=0;x<qtdEntries;x++){
        struct memory_entry *aux = &entries[x];
        uint64 qtdPages = (aux->size)/(4*KB);
        uint8 *ptr = aux->start;
        for(uint64 y=aux->lastAvaliable;y<qtdPages;y++){
            uint64 pos = y/8;
            uint8 bit = y%8;

            if(((1<<bit)&ptr[pos])==0){
                char buffer[256];
                ptr[pos]|=(1<<bit);
                auxi++;
                return aux->start+(aux->offset+y)*4*KB;
            }
        }
    }
    return 0;
}



void memzero(void* ptr, uint64 size){
    if (size % 8 == 0) {
        size /= 8;
        uint64* p64 = ptr;
        for (uint64 i = 0; i < size; i++) {
            p64[i] = 0;
        }
    } else {
        uint8* p8 = ptr;
        for (uint64 i = 0; i < size; i++) {
            p8[i] = 0;
        }
    }
}
