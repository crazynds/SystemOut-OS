
[bits 64]
%include "sorcecode/asm_kernel/funcs/start_kernel.asm"

%include "sorcecode/asm_kernel/funcs/macros.asm"
%include "sorcecode/asm_kernel/funcs/extra_utils.asm"
%include "sorcecode/asm_kernel/funcs/interruptions.asm"
%include "sorcecode/asm_kernel/funcs/load_thread.asm"
%include "sorcecode/asm_kernel/funcs/ata.asm"


[extern kmain]
[extern kloop]

start_64:
    sti
    call kmain
    ;jmp .helt
.loop:
    mov byte [TIME_SLICE],1
    hlt
    jmp .loop
.helt:
    hlt
    jmp .helt
