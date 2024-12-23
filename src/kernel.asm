global __start
[BITS 32]
CODE_SEG equ 0x8
DATA_SEG equ 0x10 
__start:
    mov ax,DATA_SEG
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov ss,ax
    mov ebp,0x00200000
    mov esp,ebp
    ;;mov eax,gdt_start
    ;Enable the A20 line
    in al,0x92
    or al,2
    out 0x92,al

    jmp $