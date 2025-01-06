#include "kernel.h"
#include "config.h"
#include <stddef.h>
#include <stdint.h>
#include "idt.h"
#include "io.h"

uint16_t *video_mem_addr;
uint16_t terminal_make_char(char c,char colour);
void terminal_put_char(int x, int y, char c, char colour);
size_t strlen(const char* str);
void terminal_writechar(char c,char colour);
void terminal_init();
void print(const char *str);

int terminal_row_idx = 0;
int terminal_column_idx = 0;

void kernel_main()
{
    video_mem_addr = (uint16_t*)(x86_VIDEO_MEM_ADDR);
    terminal_init();
    // terminal_writechar('A',4);
    //serial_write("Hello, QEMU -nographic!\n");
    // video_mem_addr[0] = terminal_make_char('C',4);
    const char *str = "Hello prashant";
    print(str);
    idt_init();
    outb(0x60,0xff);

}

uint16_t terminal_make_char(char c,char colour)
{
    return (colour << 8) | c;
}
size_t strlen(const char* str)
{
    size_t len = 0;
    while(str[len])
    {
        len++;
    }
    return len;
}

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


