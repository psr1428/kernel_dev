#include "heap.h"
#include "memory.h"

static int heap_validate_alignment(void *ptr)
{
    return ((unsigned int)ptr % HEAP_BLKSIZE) == 0;
}

static int heap_validate_table(void *end,void *ptr,struct heap_table *table)
{
    size_t table_size = (size_t)(end - ptr);
    size_t table_blks = table_size / HEAP_BLKSIZE;
    if(table->total_size != table_blks)
    {
        return -EINVARG;
    }
    return 0;
}

int heap_create(struct heap* heap,void *ptr, void *end, struct heap_table *table)
{
    int ret = 0;
    if(!heap_validate_alignment(ptr) || !heap_validate_alignment(end))
    {
        return -EINVARG;
    }
    memset(heap,0,sizeof(struct heap));
    heap->sAddr = ptr;
    heap->table = table;
    ret = heap_validate_table(end,ptr,table);
    if(ret < 0)
    {
        return ret;
    }
    size_t table_size = sizeof(HEAP_BLK_TABLE_ENT)*table->total_size;
    memset(table->entries,HEAP_BLK_TABLE_ENT_FREE,table_size);
    return 0;
}

static uint32_t heap_align_size(int size)
{
    if((size % HEAP_BLKSIZE) == 0)
    {
        return size;
    }
    size = size - (size % HEAP_BLKSIZE);
    size += HEAP_BLKSIZE;
    return size;
}

int heap_get_start_blk(struct heap *heap,int total_blks)
{
    int start_blk = -1;
    int curr_blk = 0;
    struct heap_table *table = heap->table;
    for(int i = 0; i < table->total_size; i++)
    {
        if(HEAP_GET_ENTRY_TYPE(table->entries[i]) != HEAP_BLK_TABLE_ENT_FREE)
        {
            start_blk = -1;
            curr_blk = 0;
            continue;
        }
        if(start_blk == -1)
        {
            start_blk = i;
        }
        curr_blk++;
        if(curr_blk == total_blks)
        {
            break;
        }
    } 
    if(start_blk == -1)
    {  
        return -ENOMEM;
    }
    return start_blk;
}

void *heap_get_addr(struct heap *heap,int blk)
{
    return heap->sAddr + (blk*HEAP_BLKSIZE);
}

int heap_get_blk(struct heap *heap,void *addr)
{
    return ((void*)addr - heap->sAddr)/HEAP_BLKSIZE;
}

void heap_mark_block_entry(struct heap *heap,int start_blk,int total_blks)
{
    int end_blk = (start_blk + total_blks) -1;
    HEAP_BLK_TABLE_ENT entry = HEAP_BLK_TABLE_ENT_TAKEN | HEAP_BLK_HAS_FIRST;
    if( total_blks > 1)
    {
        entry |= HEAP_BLK_HAS_NEXT;
    }
    for( int  i = start_blk; i <= end_blk; i++)
    {
        heap->table->entries[i] = entry;
        entry = HEAP_BLK_TABLE_ENT_TAKEN;
        if(i != end_blk -1)
        {
            entry |= HEAP_BLK_HAS_NEXT;
        }
    }
}

void heap_mark_block_free(struct heap *heap,int start_blk)
{
    struct heap_table *table = heap->table;
    for( int i = 0; i < table->total_size; i++)
    {
        table->entries[i] = HEAP_BLK_TABLE_ENT_FREE;
        if(!(table->entries[i] & HEAP_BLK_HAS_NEXT))
        {
            break;
        }
    }
}

void *heap_malloc_blks(struct heap *heap,uint32_t total_blks)
{
    void *addr = 0x0;
    int start_blk = heap_get_start_blk(heap,total_blks);
    if(start_blk < 0)
    {
        return addr;
    }
    addr = heap_get_addr(heap,start_blk);
    heap_mark_block_entry(heap,start_blk,total_blks);
    return addr;
}

void *heap_malloc(struct heap *heap,int size)
{
    int align_blks = heap_align_size(size) / HEAP_BLKSIZE;
    return heap_malloc_blks(heap,align_blks);
}

void heap_free(struct heap *heap,void *ptr)
{
    heap_mark_block_free(heap,heap_get_blk(heap,ptr));
}