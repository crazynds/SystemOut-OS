#ifndef THREAD_H
    #define THREAD_H

    #include "iopipe.h"

    typedef struct{
      uint64 OID;
      uint64 stack_rbp;
      uint64 stack_rsp;

      uint64 execution;

      void *x86_save_512;

      // NOT WORKING
      uint64 pagL4;

      uint16 flags; // prioridade | user/kernel | privilégio |

      output_stream *output;
    }Thread;

    extern Thread* BEFORE_THREAD;
    extern Thread* NOW_THREAD;


    void initTasking();
    uint64 createKernelThread(void* func);
    uint64 createKernelThreadArgs(void* func,void* args,uint64 tamArgs);
    void killThis();

#endif
