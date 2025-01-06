[BITS 32]
global __start
extern kernel_main
CODE_SEG equ 0x8
DATA_SEG equ 0x10 
__start:
    mov ax,DATA_SEG
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov ss,ax
    mov gs, ax
    mov ebp,0x00200000
    mov esp,ebp
    ;;mov eax,gdt_start
    ;Enable the A20 line
    in al,0x92
    or al,2
    out 0x92,al
    ;Remap the master PIC
    mov al, 00010001b
    out 0x20, al ; Tell master PIC
    ; Master PIC command port 0x20 and data port 0x21
    mov al,0x20 ; Interrupt 0x20 where master ISR should start
    out 0x21,al

    mov al, 00000001b
    out 0x21,al
    sti
    ;End of remap the master PIC
    call kernel_main

    jmp $
times 512 - ($ - $$) db 0