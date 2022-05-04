[bits 64]

[extern trocarThread]

[global BEFORE_THREAD]
[global NOW_THREAD]

IP_NOW dq 0
IP_NOW2 dq 0
NOW_THREAD dq 0
BEFORE_THREAD dq 0
TIME_SLICE db 1


alterar_task_timer:
    cli
    pushall
    call trocarThread
    cmp rax,0
    je .else
      bt rax,16
      mov rcx,0xFFFFFFFFFFFFFFF0
      mov [TIME_SLICE],al
      jnc .mode_normal

      mov rbx,[NOW_THREAD]
      mov rbp,[rbx+0x8]
      mov rsp,[rbx+0x10]
      mov rdx,[rbx+0x18]
      mov [IP_NOW],rdx

      ;jmp $

      jmp .continue

      .mode_normal:
      mov rbx,[BEFORE_THREAD]
      mov rdx,[IP_NOW]
      mov rax,[rbx+0x20]
      mov [rbx+0x8],rbp
      mov [rbx+0x10],rsp
      and rax,rcx
      mov [rbx+0x18],rdx
      fxsave [rax]

      mov rbx,[NOW_THREAD]
      mov rbp,[rbx+0x8]
      mov rsp,[rbx+0x10]
      mov rdx,[rbx+0x18]
      mov [IP_NOW],rdx


      .continue:
      lea rax,[rsp+0x10]
      cmp rax,rbp
      jne .end2
        pop rdi
        pop rsi
        mov al,0x20
        push rdx
        out 0xa0, al
        out 0x20, al
        sti
        ret
    .else:
      mov byte [TIME_SLICE],5
      jmp .end

    .end2:
      mov rax,[rbx+0x20]
      and rax,rcx
      fxrstor [rax]
    .end:
    mov al, 0x20
    popfq
    pop r15
    pop r13
    pop r14
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    out 0xa0, al
    out 0x20, al
    mov rax,[IP_NOW]
    xchg [rsp],rax
    sti
    ret






alterar_task_irq:
    cli
    pushall
    call trocarThread
    cmp rax,0
    je .else
      bt rax,16
      mov rcx,0xFFFFFFFFFFFFFFF0
      mov [TIME_SLICE],al
      jnc .mode_normal

      mov rbx,[NOW_THREAD]
      mov rbp,[rbx+0x8]
      mov rsp,[rbx+0x10]
      mov rdx,[rbx+0x18]
      mov [IP_NOW2],rdx

      ;jmp $

      jmp .continue

      .mode_normal:
      mov rbx,[BEFORE_THREAD]
      mov rdx,[IP_NOW2]
      mov rax,[rbx+0x20]
      mov [rbx+0x8],rbp
      mov [rbx+0x10],rsp
      and rax,rcx
      mov [rbx+0x18],rdx
      fxsave [rax]

      mov rbx,[NOW_THREAD]
      mov rbp,[rbx+0x8]
      mov rsp,[rbx+0x10]
      mov rdx,[rbx+0x18]
      mov [IP_NOW2],rdx

      .continue:
      lea rax,[rsp+0x10]
      cmp rax,rbp
      jne .end
        pop rdi
        pop rsi
        push rdx
        sti
        ret
    .else:
      mov byte [TIME_SLICE],1
    .end:
    popfq
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rdx
    mov rax,[IP_NOW2]
    pop rcx
    pop rbx
    xchg [rsp],rax
    sti
    ret
