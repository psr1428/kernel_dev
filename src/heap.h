#ifndef HEAP_H
#define HEAP_H

#include "config.h"
#include <stddef.h>

#define HEAP_BLK_TABLE_ENT_TAKEN 0x1
#define HEAP_BLK_TABLE_ENT_FREE 0x0

#define HEAP_BLK_HAS_NEXT 0b10000000
#define HEAP_BLK_HAS_FIRST 0b01000000


typedef unsigned char HEAP_BLK_TABLE_ENT;

#define HEAP_GET_ENTRY_TYPE(ENTRY) (ENTRY & 0xf)

struct heap_table
{
    HEAP_BLK_TABLE_ENT *entries;
    size_t total_size;
};

struct heap
{
    struct heap_table *table;
    void *sAddr;/*Start address of heap table*/
};
int heap_create(struct heap* heap,void *ptr, void *end, struct heap_table *table);
void *heap_malloc(struct heap *heap,int size);
void heap_free(struct heap *heap,void *ptr);
#endif