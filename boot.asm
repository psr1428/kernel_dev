ORG 0x7c00
; section .text
BITS 16
; extern load_protecetd
CODE_SEG equ gdt_code-gdt_start
DATA_SEG equ gdt_data-gdt_start

__start:
    jmp short start_section
    nop
times 33 db 0

; start:
;     jmp 0x7c0:start_section

start_section:
    cli ;Clear interrupts
    mov ax,0
    ; mov ax,0x6c0
    mov ds,ax
    mov es,ax
    mov ax,0x0
    mov ss,ax
    mov sp,0x7c00
    sti;Enables interrupts

.load_protecetd:
    cli
    lgdt[gdt_desc]
    mov eax,cr0
    or eax,0x1
    mov cr0,eax
    jmp CODE_SEG:load32
gdt_start:
gdt_null:
    dd 0x0
    dd 0x0
;offset 0x8
gdt_code: ;Code segment
    dw 0xFFFF   ;segmnent limit first 0-15bits
    dw 0        ;base first 0-15 bits
    db 0        ;base 16-23 bits
    db 0x9a     ;access byte
    db 11001111b    ;high 4 bit flags and the low 4 bit flags
    db 0            ; base 24-31 bits
;offset 0x10
gdt_data:   ;data segment,ss,es
    dw 0xFFFF   ;segmnent limit first 0-15bits
    dw 0        ;base first 0-15 bits
    db 0        ;base 16-23 bits
    db 0x92     ;access byte
    db 11001111b    ;high 4 bit flags and the low 4 bit flags
    db 0            ; base 24-31 bits
gdt_end:
gdt_desc:
    dw gdt_end-gdt_start-1
    dd  gdt_start

[BITS 32]
load32:
    jmp $

error:
    mov si,error_msg
    call print
    jmp $
print:
    mov bx,0
.loop:
    lodsb
    cmp al,0
    je .done
    call print_char
    jmp .loop
.done:
    ret
print_char:
    mov ah,0eh
    int 0x10
    ret
error_msg: db 'Fail to load disk',0
message: db 'Hello world',0
times 510 - ($ - $$) db 0
dw 0xAA55
buffer:
;test_buffer: db 'Hellllllo',0