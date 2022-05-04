
[global idt_register_isrs]
[global FREQUENCY]

[extern idt_set_interrupt_gate]
[extern kprint]
[extern keyboard]
[extern page_fault_handler]
[extern kprintnum2]
[extern kprintnum3]


idt_register_isrs:

    generic_exception 0,  "divide by zero"
    generic_exception 1,  "debug"
    generic_exception 2,  "non-maskable interrupt"
    generic_exception 3,  "breakpoint"
    generic_exception 4,  "overflow"
    generic_exception 5,  "bound range exceeded"
    generic_exception 6,  "invalid opcode"
    generic_exception 7,  "device not available"
    generic_exception 8,  "double fault"
    generic_exception 10, "invalid tss"
    generic_exception 11, "segment not present"
    generic_exception 12, "stack segment fault"
    generic_exception 13, "general protection fault"

    ; PAGE FAULT 14
    begin_isr 14
        pushaq
        mov rsi,cr2
        call page_fault_handler
        int 0x29
        popaq
    end_isr

    ; IRQ 0 - PIT
    begin_isr 32
        ; 6 valores armazenados na stack
        ; sendo o 3 valor o ponteiro para retornar para o processo normal
        ; testar usando apenas os 3 primeiros
        ; depois progredir


        pushall
          inc qword [VAL]
          mov rbx,[FREQUENCY]
          mov rax,[VAL]
          xor rdx,rdx
          dec byte [TIME_SLICE]
          div rbx
          mov rdi,rax
          call kprintnum2
        popall

        cmp byte [TIME_SLICE],0
        jnz .end_timer
        push rax
          mov rax,[rsp+0x8]
          mov [IP_NOW],rax
          mov rax,alterar_task_timer
          mov [rsp+0x8],rax
        pop rax
        jmp .end_timer2
        .end_timer:
        push rax
          mov al, 0x20
          out 0xa0, al
          out 0x20, al
        pop rax
        .end_timer2:
    end_isr

    ; IRQ 1 - keyboard
    begin_isr 33
        pushall

        in al,0x60
        mov edi,eax
        call keyboard

        mov al, 0x20
        out 0xa0, al
        out 0x20, al

        popall
    end_isr

    ; IRQ 7 - spurious
    begin_isr 39
        push rax
        mov al, 0x20
        out 0xa0, al
        out 0x20, al
        pop rax
    end_isr

    ; IRQ 14 - ATA Primario
    begin_isr 46
        push rax
          mov al, 0x20
          out 0xa0, al
          out 0x20, al
          pop rax
    end_isr
    ; IRQ 15 - ATA Secundario
    begin_isr 47
        push rax
        mov al, 0x20
        out 0xa0, al
        out 0x20, al
        pop rax
    end_isr







    ; Troca de treads -  A thread atual cede seu espaço para outra thread
    begin_isr 0x70
      push rax
        mov rax,[rsp+0x8]
        mov [IP_NOW2],rax
        mov rax,alterar_task_irq
        mov [rsp+0x8],rax
      pop rax
    end_isr
ret

FREQUENCY dq 50
VAL dq 0
