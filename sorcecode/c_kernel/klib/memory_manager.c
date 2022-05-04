#include "../header/Memory.h"
#include "../header/Variaveis.h"
#include "../lib/String.h"

#define ID_SUPER 0x76ad561b


#define PRO_MAX 64
// define 1048576 espacos maximos de memoria
//#define PRO_MAX 1024
// define 1024 espaÃ§os mÃ¡ximos de memÃ³ria
//#define PRO_MAX 256
// define 256 espaÃ§os mÃ¡ximos de memÃ³ria

//Todos os tipos de blocos funcionam
#define BLOCO 8

// 16 Bytes | 128 bits
typedef struct{
    void *pos;
    uint32 tam;
    uint32 id;
}mem_Ocup;

//Variaveis de controle da memoria (Alocada + mapa de disponiveis + memorias ativas)
uint8 *mem_public;
uint8 *mem_map;
mem_Ocup programs[PRO_MAX];

//Variaveis auxiliares (ponteiro da memoria disponivel mais próxima + id auxiliar + endereço para void)
uint32 ID_AUX_MEM=65536;
uint32 PONT_AUX_MEM=0;
uint64 END_VOID[10];

uint64 MEM_MAX;

void memoryManagerStart(uint64 memoryStart, uint64 mBytes){
  MEM_MAX=(mBytes-8)*(BLOCO<<3);
  MEM_MAX/=1+(BLOCO<<3);
  mem_public=(void*)memoryStart;
  mem_map=(void*)(memoryStart+MEM_MAX+8);
  resetMem();
}
inline int getBit(uint8 alvo,int pos){
    if(pos>7||pos<0){
        return -1;
    }
    pos++;
    alvo=alvo<<(8-pos);
    alvo=alvo>>7;
    return alvo;
}
boolean isSpace(uint32 val){
    uint64 p=0,y,aux=0;
    int64 x;
    for(x=0;x<PRO_MAX && aux<3;x++){
        if(programs[x].pos==0)aux+=1;
    }
    if(aux<3)return false;
    for(x=MEM_MAX/(8*BLOCO)-1;x>=0 && p*BLOCO<=val;x--){
        for(y=0;y<8;y++){
            if(getBit(mem_map[x],y)==0){
                p++;
            }else{
                p=0;
            }
        }
    }
    if(p*BLOCO>val)return true;
    else return false;
}

void* getEndPublic(){
    return mem_public;
}
void* getEndVoid(){
    return &END_VOID;
}
void statusMem(){
    uint64 p=0,x,y=0,aux=MEM_MAX/((MEM_MAX<256)?MEM_MAX:256),aux2=0;
    //printf("\nUso de memoria: [");
    kprintStr(0,20,"Uso da memoria: ",0x1f);
    int ky=0;
    for(x=0;x<MEM_MAX/(8*BLOCO);x+=aux/BLOCO+1){
        for(y=aux2;y<8;y+=aux){
            if(getBit(mem_map[x],y)==1){
              kprintStr(ky++,21,"|",0x1f);
            }else kprintStr(ky++,21,"_",0x1f);
        }
        aux2=y%8;
    }
    for(x=0;x<MEM_MAX/(8*BLOCO);x++){
        for(y=0;y<8;y++){
            if(getBit(mem_map[x],y)==1){
                p++;
            }
        }
    }
    char buffer[256];
    sprintf(buffer,"Memoria usada: %d%% => %d blocos => %d bytes",(int)(100*(float)p/MEM_MAX),p,p*BLOCO);
    kprintStr(0,22,buffer,0x0f);
    sprintf(buffer,"Memoria maxima: %d",MEM_MAX);
    kprintStr(0,23,buffer,0x0f);
    //printf("] %.2f%% %ld bytes",100*(float)p/MEM_MAX,p*BLOCO);
    for(p=0,x=0;x<PRO_MAX;x++){
       if(programs[x].pos!=0)p++;
    }
    //printf("\nVariaveis alocadas: %ld (%.2f%%) \n\n",p,(100*p/(double)PRO_MAX));
}
//*/
void resetMem(){
    uint64 x;
    PONT_AUX_MEM=0;
    for(x=0;x<MEM_MAX/(8*BLOCO);x++){
        mem_map[x]=0;
    }
    for(x=0;x<PRO_MAX;x++){
        programs[x].pos=0;
    }
}
/*
void mostrarMem(){
    int x,y;
    for(x=0;x<MEM_MAX/(8*BLOCO);x++){
        for(y=0;y<8;y++){
                printf("%d-",getBit(mem_map[x],y));
        }
        printf("\n");
    }
    for(x=0;x<MEM_MAX;x++){
        if(x%8==0)printf("|");
        printf(" %c ",mem_public[x]);
    }
    printf("\n");
}*/

void _libera(int w){
    uint64 tam=programs[w].tam,aux,y;
    {
        uint64 p1=(uint64)programs[w].pos, p2=(uint64)mem_public;
        aux = (p1-p2)/BLOCO;
        if(aux<PONT_AUX_MEM)PONT_AUX_MEM=aux;
    }
    for(y=0;y<tam;y++){
        int z=aux+y;
        int a=z%8,b=z/8;
        if(getBit(mem_map[b],a)!=0){
            mem_map[b]-=1<<a;
         }
    }
    programs[w].pos=0;
    programs[w].tam=0;
}

void libera(void *p){
    uint64 w=0;
    if(p==0)return;
    if(p==&END_VOID)return;
    while(programs[w].pos!= p){
        w++;
        if(w>=PRO_MAX)return;
    }
    _libera(w);
}
void liberaId(uint32 id){
    if(id==0)return;
    uint64 w=0;
    while(w<PRO_MAX){
        if(programs[w].id==id){
            _libera(w);
        }
        w++;
    }
}
void liberaSuper(){
    liberaId(ID_SUPER);
}
uint32 getId(){
    if(0xFFFFFFF8<ID_AUX_MEM)ID_AUX_MEM=1026;
    return ID_AUX_MEM++;
}
boolean verId(uint32 id){
    boolean aux=false;
    uint64 w=0;
    do{
        if(programs[w].id==id){
            aux=true;
        }
        w++;
    }while(aux==false && w<PRO_MAX);
    return aux;
}
uint32 getIdVer(){
    uint32 val;
    do{
        val=getId();
    }while(verId(val));
    return val;
}

void* alocaId(uint32 tam,uint32 id){
    uint64 x=PONT_AUX_MEM,y,w=0;
    boolean bol=true,bolPont=true;
    if(tam<=0)return &END_VOID;
    if(tam%BLOCO>0)tam=tam/BLOCO+1;
    else tam/=BLOCO;
    do{
        if(!getBit(mem_map[x/8],x%8)){
            boolean bol2=true;
            if(bolPont){
                PONT_AUX_MEM=x;
                bolPont=false;
            }
            __asm__ ("cli");
            y=x;
            do{
                y++;
                if(getBit(mem_map[y/8],y%8)){
                    bol2=false;
                    break;
                }
            }while(y-x<tam);
            if(bol2){
                if(x==PONT_AUX_MEM)PONT_AUX_MEM+=tam;
                for(y=0;y<tam;y++){
                    uint64 z=x+y;
                    uint64 a=z%8,b=z/8;
                    if(getBit(mem_map[b],a)!=1){
                        mem_map[b]+=1<<a;
                    }
                }
                bol=false;
            }else x++;
            __asm__ ("sti");
        }else x++;
        if(x>MEM_MAX/BLOCO-tam){
            x=0;
        }
    }while(bol);
    while(programs[w].pos!=0){
        w++;
        if(w>=PRO_MAX)w=0;
    }
    programs[w].pos=&mem_public[x*BLOCO];
    programs[w].tam=tam;
    programs[w].id=id;
    return &mem_public[x*BLOCO];
}
void* alocaSuper(uint32 tam){
    return alocaId(tam,ID_SUPER);
}
void* alocaTemp(uint32 tam){
    uint64 x=MEM_MAX/BLOCO-tam,y;
    boolean bol=true;
    if(tam<=0)return &END_VOID;
    if(tam%BLOCO>0)tam=tam/BLOCO+1;
    else tam/=BLOCO;
    do{
        if(!getBit(mem_map[x/8],x%8)){
            __asm__ ("cli");
            bol=false;
            y=1;
            do{
                uint64 z=x+y;
                if(getBit(mem_map[z/8],z%8))bol=true;
                y++;
            }while(y<tam && !bol);
            __asm__ ("sti");
        }
        if(bol)x--;
        if(x<=0){
            x=MEM_MAX/BLOCO-tam;
        }
    }while(bol);
    return &mem_public[x*BLOCO];
}
void* aloca(uint32 tam){
    return alocaId(tam,0);
}
void* maloca(uint32 tam){
    char *p=(char*)aloca(tam);
    uint32 x;
    for(x=0;x<tam;x++)p[x]=0;
    return p;
}
void* realoca(void *p,uint32 tam){
    uint64 w=0;
    while(programs[w].pos!= p){
        w++;
        if(w>=PRO_MAX){
            return aloca(tam);
        }
    }
    if(tam<=0){
        libera(p);
        return &END_VOID;
    }
    unsigned long aux=programs[w].tam,x;
    unsigned long id=programs[w].id;
    if(BLOCO==8){
        int64 *lp,*c=(int64*)p;
        lp=(int64*)alocaId(tam,id);
        libera(p);
        if(tam%BLOCO>0)tam=(tam/BLOCO)+1;
        else tam/=BLOCO;
        for(x=0;x<tam && x<aux;x++)lp[x]=c[x];
        return (void*)lp;
    }else if(BLOCO==4){
        int32 *lp,*c=(int32*)p;
        lp=(int32*)alocaId(tam,id);
        libera(p);
        if(tam%BLOCO>0)tam=(tam/BLOCO)+1;
        else tam/=BLOCO;
        for(x=0;x<tam && x<aux;x++)lp[x]=c[x];
        return (void*)lp;
    }else if(BLOCO==2){
        int16 *lp,*c=(int16*)p;
        lp=(int16*)alocaId(tam,id);
        libera(p);
        if(tam%BLOCO>0)tam=(tam/BLOCO)+1;
        else tam/=BLOCO;
        for(x=0;x<tam && x<aux;x++)lp[x]=c[x];
        return (void*)lp;
    }else{
        aux*=BLOCO;
        char *lp,*c=(char*)p;
        lp=(char*)alocaId(tam,id);
        libera(p);
        for(x=0;x<tam && x<aux;x++)lp[x]=c[x];
        return (void*)lp;
    }

}
void* alocaCop(void *p,uint32 ids){
    uint64 w=0;
    while(programs[w].pos!= p){
        w++;
        if(w>=PRO_MAX){
            return &END_VOID;
        }
    }
    unsigned long aux=programs[w].tam,x;

    if(BLOCO==8){
        int64 *lp,*c=(int64*)p;
        lp=(int64*)alocaId(aux,ids);
        if(aux%8>0)aux=(aux/8)+1;
        else aux/=8;
        for(x=0;x<aux;x++)lp[x]=c[x];
        return (void*)lp;
    }else if(BLOCO==4){
        int32 *lp,*c=(int32*)p;
        lp=(int32*)alocaId(aux,ids);
        if(aux%4>0)aux=(aux/4)+1;
        else aux/=4;
        for(x=0;x<aux;x++)lp[x]=c[x];
        return (void*)lp;
    }else if(BLOCO==2){
        int16 *lp,*c=(int16*)p;
        lp=(int16*)alocaId(aux,ids);
        if(aux%2>0)aux=(aux/2)+1;
        else aux/=2;
        for(x=0;x<aux;x++)lp[x]=c[x];
        return (void*)lp;
    }else{
        int8 *lp,*c=(int8*)p;
        lp=(int8*)alocaId(aux,ids);
        for(x=0;x<aux;x++)lp[x]=c[x];
        return (void*)lp;
    }

}
