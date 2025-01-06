section .asm

global insw
global insb
global outb
global outw

insb:
    push ebp
    mov ebp,esp
    
    xor eax,eax
    mov edx, [edx + 8]
    in al,dx
    pop ebp
    ret

insw:
    push ebp
    mov ebp,esp
    
    xor eax,eax
    mov edx, [edx + 8]
    in ax,dx
    pop ebp
    ret
outb:
    push ebp
    mov ebp,esp

    xor eax,eax
    mov dx,[ebp+8]
    mov al,[ebp+12]
    out dx,al
    pop ebp
    ret
outw:
    push ebp
    mov ebp,esp

    xor eax,eax
    mov dx,[ebp+8]
    mov ax,[ebp+12]
    out dx,ax 
    pop ebp
    ret
