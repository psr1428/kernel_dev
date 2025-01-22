#include "kernel.h"
#include "config.h"
#include <stddef.h>
#include <stdint.h>
#include "idt.h"
#include "io.h"
#include "kheap.h"
#include "paging.h"
#include "disk.h"
#include "string.h"
#include "pathparser.h"
#include "file.h"
#include "memory.h"

uint16_t *video_mem_addr;
uint16_t terminal_make_char(char c,char colour);
void terminal_put_char(int x, int y, char c, char colour);
// size_t strlen(const char* str);
void terminal_writechar(char c,char colour);
void terminal_init();
void print(const char *str);

int terminal_row_idx = 0;
int terminal_column_idx = 0;

static struct paging_chunk *kernel_chunk = 0;

void kernel_main()
{
    video_mem_addr = (uint16_t*)(x86_VIDEO_MEM_ADDR);
    terminal_init();
    // terminal_writechar('A',4);
    //serial_write("Hello, QEMU -nographic!\n");
    // video_mem_addr[0] = terminal_make_char('C',4);
    // const char *str = "Hello prashant";
    // print(str);
    kheap_init();
    // Initialize filesystems
    fs_init();
    disk_search_and_init();
    idt_init();

    // char *heap_mem = kernel_zalloc(4096);
    // memset(heap_mem,0,4096);
    kernel_chunk = paging_new(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    paging_switch(paging_get_directory(kernel_chunk));
    // paging_set(paging_get_directory(kernel_chunk), (void*)0x2000, (uint32_t)heap_mem | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITEABLE);
    enable_paging();
    enable_interrupts();
    int fd = fopen("0:/hello.txt", "r");
    char bufread[20];
    if (fd)
    {
        print("We opened hello.txt\n");
        fseek(fd,5,SEEK_SET);
        fread(bufread,5,1,fd);
        bufread[20] = 0x0;
        print(bufread);
    }
    //struct path_root *root_path = path_parse("0:/bin/shell.exe",NULL);
    //if(root_path)
    //{

    //}
    while(1) {}
#if 0
    char *ptr2 = (char*)0x1000;
    ptr2[0] = 'P';
    ptr2[1] = 'R';
    print(ptr2);
    print(heap_mem);
    char buf[512];
    disk_read_sector(0,1,buf);
    void *test_ptr1 = kernel_malloc(50);
    kernel_free(test_ptr1);
    void *test_ptr2 = kernel_malloc(100);
    if(test_ptr1 || test_ptr2)
    {
        print("Received Heap allocation\n");
    }
    // outb(0x60,0xff);
#endif

}

uint16_t terminal_make_char(char c,char colour)
{
    return (colour << 8) | c;
}
// size_t strlen(const char* str)
// {
//     size_t len = 0;
//     while(str[len])
//     {
//         len++;
//     }
//     return len;
// }

void print(const char *str)
{
    int len = strlen(str);
    for(int i = 0; i < len;i++)
    {
        terminal_writechar(str[i],4);
    }
}


void terminal_writechar(char c,char colour)
{
    if(c == '\n')
    {
        terminal_column_idx = 0;
        terminal_row_idx +=1;
        return;
    }

    terminal_put_char(terminal_column_idx,terminal_row_idx,c,colour);
    terminal_column_idx+=1;
    if(terminal_column_idx >=VGA_WIDTH)
    {
        terminal_column_idx = 0;
        terminal_row_idx+=1;
    }

}

void terminal_put_char(int x, int y, char c, char colour)
{
    video_mem_addr[(y*VGA_WIDTH) + x] = terminal_make_char(c,colour);
}

void terminal_init()
{
    for(int y = 0; y < VGA_HEIGHT; y++)
    {
        for(int x = 0; x <VGA_WIDTH; x++)
        {
            terminal_put_char(x,y,' ',0);
        }
    }
}


