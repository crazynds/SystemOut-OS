
[global sleep]
[global outb]
[global inb]
[global gdt_fluh]
[global sprintf]
[global printf]
[global yield]
[global tss_flush]

tss_flush:
    mov ax, 0x2B
;    ltr ax
    ret

gdt_fluh:
    lgdt [rdi]
    ret


    jmp enter_user_mode

    .continuos:

    jmp $

    ret

enter_user_mode:
    mov rdi,rsp
    push 0x10
    push rdi
    pushfq
    push 0x8
    push gdt_fluh.continuos

    iretq


outb:
    mov edx,edi
    mov eax,esi
    out dx,al
    ret

inb:
    mov edx,edi
    xor eax,eax
    in al,dx
    ret

sleep:
    pushall
    mov rbx,[FREQUENCY]
    mov rax,[VAL]
    xor rdx,rdx
    div rbx
    mov r8,rax
    add r8,rdi
    .loop:
      int 0x70
      mov rbx,[FREQUENCY]
      mov rax,[VAL]
      xor rdx,rdx
      div rbx
      cmp r8,rax
      ja .loop
    popall
    ret

[extern _sprintf]
[extern _printf]
[extern getChar]

yield:
    int 0x70
    ret

printf:
  pop rax

  push r9
  push r8
  push rcx
  push rdx
  push rsi

  mov rsi,rsp

  push rax
  call _printf


  pop rax
  sub rsp,0x20
  push rax
  ret

sprintf:
  pop rax
  push r9
  push r8
  push rcx
  push rdx

  mov rdx,rsp

  push rax

  call _sprintf

  pop rdx
  add rsp,0x20
  push rdx

  ret
