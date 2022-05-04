
[global identify_device]
[global BUFFER_HD]

BUFFER_HD dq 0

; RDI = Device pointer
identify_device:
  movzx  rbx, word [rdi]
  lea dx,[rbx+2]
  mov ax,0
  out dx,al
  inc dx
  out dx,al
  inc dx
  out dx,al
  inc dx
  out dx,al
  add dx,2
  mov al,0xEC
  out dx,al
  in al,dx
  cmp al,0
  je .not_valid
.loop:
  in al,dx
  bt ax,7   ;Verifica se a unidade não apresenta erro
  jc .loop
  bt ax,3   ;Verifica se a unidade está pronta para enviar ou receber dados
  jnc .loop

  mov rcx, 128
  mov dx,bx
  mov rdi,BUFFER_HD
  rep insd             ; in to [RDI]

  mov word [rdi+0x3],1
  ret


.not_valid:
  mov word [rdi+0x2],0
  ret






;=============================================================================
; ATA read sectors (LBA mode)
;
; @param EAX Logical Block Address of sector
; @param CL  Number of sectors to read
; @param RDI The address of buffer to put data obtained from disk
;
; @return None
;=============================================================================
ata_lba_read:
     pushfq
     push rax
     and rax, 0x0FFFFFFF
     push rcx
     push rdx
     push rdi
     push rbx

     mov rdx,rbx

     mov rbx, rax         ; Save LBA in RBX

     ;mov edx, 0x01F6      ; Port to send drive and bit 24 - 27 of LBA
     add rdx,6            ; Port 6
     shr eax, 24          ; Get bit 24 - 27 in al
     or al, 11100000b     ; Set bit 6 in al for LBA mode
     out dx, al

     ;mov edx, 0x01F2      ; Port to send number of sectors
     sub rdx,4            ; Port 2
     mov al, cl           ; Get number of sectors from CL
     out dx, al

     ;mov edx, 0x1F3       ; Port to send bit 0 - 7 of LBA
     inc rdx              ; Port 3
     mov eax, ebx         ; Get LBA from EBX
     out dx, al

     ;mov edx, 0x1F4       ; Port to send bit 8 - 15 of LBA
     inc rdx              ; Port 4
     mov eax, ebx         ; Get LBA from EBX
     shr eax, 8           ; Get bit 8 - 15 in AL
     out dx, al


     ;mov edx, 0x1F5       ; Port to send bit 16 - 23 of LBA
     inc rdx              ; Port 5
     mov eax, ebx         ; Get LBA from EBX
     shr eax, 16          ; Get bit 16 - 23 in AL
     out dx, al

     ;mov edx, 0x1F7       ; Command port
     add rdx,2
     mov al, 0x20         ; Read with retry.
     out dx, al

.still_going:
     in al, dx
     test al, 8           ; the sector buffer requires servicing.
     jz .still_going      ; until the sector buffer is ready.

     mov rax, 128         ; to read 256 words = 1 sector
     cmp cl,1
     je .not_mul
     xor bx, bx
     mov bl, cl           ; read CL sectors
     mul bx
.not_mul:
     pop rdx
     mov rcx, rax         ; RCX is counter for INSW
     rep insd             ; in to [RDI]

     pop rdi
     pop rdx
     pop rcx
     pop rax
     popfq
     ret


;=============================================================================
; ATA write sectors (LBA mode)
;
; @param RAX Logical Block Address of sector
; @param RBX qual driver a ser escrito         //0x1f0
; @param CL  Number of sectors to write
; @param RDI The address of data to write to the disk
; @return None
;=============================================================================

ata_lba_write:
    pushfq
    push rax
    and rax, 0x0FFFFFFF
    push rcx
    push rdx
    push rdi
    push rbx
    mov rdx, rbx
    mov rbx, rax         ; Save LBA in RBX

    add rdx,6            ; Port 6
    mov rdx,0x1f6
    shr eax, 24          ; Get bit 24 - 27 in al
    or al, 11100000b     ; Set bit 6 in al for LBA mode
    out dx, al

    sub rdx,4            ; Port 2
    mov rdx,0x1f2
    mov al, cl           ; Get number of sectors from CL
    out dx, al

    inc rdx              ; Port 3
    mov rdx,0x1f3
    mov eax, ebx         ; Get LBA from EBX
    out dx, al

    inc rdx              ; Port 4
    mov rdx,0x1f4
    mov eax, ebx         ; Get LBA from EBX
    shr eax, 8           ; Get bit 8 - 15 in AL
    out dx, al

    inc rdx              ; Port 5
    mov rdx,0x1f5
    mov eax, ebx         ; Get LBA from EBX
    shr eax, 16          ; Get bit 16 - 23 in AL
    out dx, al

    add rdx, 2           ; Port 7
    mov rdx,0x1f7
    mov al, 0x30         ; Write with retry.
    out dx, al

.still_going:
    in al, dx
    test al, 8           ; the sector buffer requires servicing.
    jz .still_going      ; until the sector buffer is ready.

    mov rax,128          ; to read 128 dwords = 1 sector
    cmp cl,1
    jbe .not_mul
    xor bx, bx
    mov bl, cl           ; write CL sectors
    mul bx
.not_mul:
    pop rdx
    mov rcx, rax         ; RCX is counter for OUTSW
    mov rsi, rdi
    rep outsd           ; out

    pop rdi
    pop rdx
    pop rcx
    pop rax
    popfq
    ret
