#include "streamer.h"
#include "kheap.h"
#include "config.h"

struct disk_stream* diskstreamer_new(int disk_id)
{
    struct disk* disk = disk_get(disk_id);
    if (!disk)
    {
        return 0;
    }

    struct disk_stream* streamer = kernel_zalloc(sizeof(struct disk_stream));
    streamer->pos = 0;
    streamer->disk = disk;
    return streamer;
}

int diskstreamer_seek(struct disk_stream* stream, int pos)
{
    stream->pos = pos;
    return 0;
}

int diskstreamer_read(struct disk_stream* stream, void* out, int total)
{
    int sector = stream->pos / DISK_SECTOR_SIZE;
    int offset = stream->pos % DISK_SECTOR_SIZE;
    char buf[DISK_SECTOR_SIZE];

    int res = disk_read_block(stream->disk, sector, 1, buf);
    if (res < 0)
    {
        goto out;
    }

    int total_to_read = total > DISK_SECTOR_SIZE ? DISK_SECTOR_SIZE : total;
    for (int i = 0; i < total_to_read; i++)
    {
        *(char*)out++ = buf[offset+i];
    }

    // Adjust the stream
    stream->pos += total_to_read;
    if (total > DISK_SECTOR_SIZE)
    {
        res = diskstreamer_read(stream, out, total-DISK_SECTOR_SIZE);
    }
out:
    return res;
}

void diskstreamer_close(struct disk_stream* stream)
{
    kernel_free(stream);
}