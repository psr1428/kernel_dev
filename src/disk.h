#ifndef DISK_H
#define DISK_H

typedef unsigned int DISK_TYPE;

#define DISK_TYPE_REAL  0

struct  disk
{
    DISK_TYPE type;
    int sector_size;
};


int disk_read_blk(struct disk *idisk,int lba,int total, void *buf);
struct disk* disk_get(int idx);
void disk_search_and_init();
#endif