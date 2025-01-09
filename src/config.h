#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

#define KERNEL_CODE_SELECTOR 0x8
#define TOTAL_INTERRUPTS  512
#define x86_VIDEO_MEM_ADDR 0xB8000

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#define HEAP_SIZE_BYTES  104857600
#define HEAP_BLKSIZE     4096
#define HEAP_ADDRESS     0x01000000	
#define HEAP_TABLE_ADDR  0x00007E00

#define DISK_SECTOR_SIZE    512

#define KRNL_OK 0
#define EIO     1
#define EINVARG 2
#define ENOMEM  3
#define EBADPATH 4
#endif
