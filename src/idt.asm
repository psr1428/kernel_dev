section .asm

extern kboard_handler
extern noint_handler
global idt_load
global int21h
global no_int
idt_load:
	push ebp
	mov ebp,esp
	mov ebx,[ebp+8]
	lidt [ebx]
	pop ebp
	ret
int21h:
	pushad
	cli
	call kboard_handler
	popad
	sti
	iret
no_int:
	pushad
	cli
	call noint_handler
	popad
	sti
	iret