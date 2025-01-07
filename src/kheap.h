#ifndef KHEAP_H
#define KHEAP_H

#include "config.h" 
#include <stddef.h>

void kheap_init();
void *kernel_malloc(size_t size);
void kernel_free(void *ptr);
#endif