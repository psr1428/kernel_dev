#ifndef IDT_H
#define IDT_H

#include "config.h"

struct idt_desc
{
	uint16_t lOffset; //offset bits 0-15
	uint16_t selector;//selector in GDT
	uint8_t zero; //unused 
	uint8_t type_attr; //desc type and attr
	uint16_t hOffset; //offset bits 16-31
}__attribute__((packed));

struct idtr_desc
{
	uint16_t limit; //size of desc table -1
	uint32_t base_addr; //base addres of idt table
}__attribute__((packed));

void idt_init();
void enable_interrupts();
void disable_interrupts();
#endif
