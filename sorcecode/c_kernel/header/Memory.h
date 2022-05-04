#ifndef MEMORY_H
    #define MEMORY_H

    #define KB 1024
    #define MB 1024*KB
    #define GB 1024*MB


    struct memory_entry{
        uint64 start;
        uint64 size;
        uint64 offset;
        uint64 lastAvaliable;
        uint16 leftoverStart;
        uint16 leftoverSize;
    }__attribute__((packed));


    uint32 addMemoryEntry(uint64 start,uint64 maxSize);
    struct memory_entry* getMemoryEntry(uint16 x);
    void* allocPage();
    void freePage(void *ptr);
    uint64 pagesOcupeds();
    
    void memzero(void*, uint64);
    void resetMem();


    //void memoryManagerStart(uint64,uint64);
    

#endif
