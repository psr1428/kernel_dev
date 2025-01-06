#include "memory.h"
#include "idt.h"
#include "kernel.h"
#include "io.h"

struct idt_desc idt_desc_table[TOTAL_INTERRUPTS];
struct idtr_desc idtr_desc_table;

extern void idt_load(struct idtr_desc *ptr);
extern void int21h();
extern void no_int();
void int_zero()
{
	print("Divide by zero error\n");
}

void kboard_handler()
{
	print("keyboard interrupt triggered\n");
	outb(0x20,0x20);
}

void noint_handler()
{
	outb(0x20,0x20);
}

void set_idt(int int_no,void *addr)
{
	struct idt_desc* desc = &idt_desc_table[int_no];
	desc->lOffset = (uint32_t)addr & 0x0000ffff;
	desc->selector = KERNEL_CODE_SELECTOR;
	desc->zero = 0x00;
	desc->type_attr = 0xEE;
	desc->hOffset = (uint32_t)addr >> 16;
}	

void idt_init()
{
	memset(idt_desc_table,0,sizeof(idt_desc_table));
	idtr_desc_table.limit = sizeof(idt_desc_table)-1;
	idtr_desc_table.base_addr = (uint32_t)idt_desc_table;
	for(int i = 0; i < TOTAL_INTERRUPTS; i++)
	{
		set_idt(i,no_int);
	}
	set_idt(0,int_zero);	
	set_idt(0x21,int21h);
	idt_load(&idtr_desc_table);
}
