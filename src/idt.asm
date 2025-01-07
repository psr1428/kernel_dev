section .asm

extern kboard_handler
extern noint_handler

global idt_load
global int21h
global no_int
global enable_interrupts
enable_interrupts:
	sti
	ret
disable_interrupts:
	cli
	ret
	
idt_load:
	push ebp
	mov ebp,esp
	mov ebx,[ebp+8]
	lidt [ebx]
	pop ebp
	ret
int21h:
	cli
	pushad
	call kboard_handler
	popad
	sti
	iret
no_int:
	cli
	pushad
	call noint_handler
	popad
	sti
	iret