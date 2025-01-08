#include "kheap.h"
#include "heap.h"
#include "kernel.h"
#include "memory.h"

struct heap krnl_heap;
struct heap_table krnl_heap_table;

void kheap_init()
{
    int total_table_entries = HEAP_SIZE_BYTES / HEAP_BLKSIZE;
    krnl_heap_table.entries = (HEAP_BLK_TABLE_ENT *)HEAP_TABLE_ADDR;
    krnl_heap_table.total_size = total_table_entries;

    void *end = (void*)(HEAP_ADDRESS + HEAP_SIZE_BYTES);
    int res = heap_create(&krnl_heap,(void*)HEAP_ADDRESS,end,&krnl_heap_table);
    if(res < 0)
    {
        print("Fail to create heap\n");
    }
}

void *kernel_malloc(size_t size)
{
    return heap_malloc(&krnl_heap,size);
}

void kernel_free(void *ptr)
{
    heap_free(&krnl_heap,ptr);
}

void *kernel_zalloc(size_t size)
{
    void* ptr = kernel_malloc(size);
    if (!ptr)
        return 0;

    memset(ptr, 0x00, size);
    return ptr;
}