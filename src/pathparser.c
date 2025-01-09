#include "pathparser.h"
#include "string.h"
#include "kheap.h"

static int valid_path_format(const char* filename)
{
    int len = strlen(filename);
    return (len >= 3 && isDigit(filename[0]) && memcmp((void*)&filename[1],":/",2)== 0);
}

static int get_drv_by_path(const char **path)
{
    if(!valid_path_format(*path))
    {
        return -EBADPATH;
    }
    int drv_no = tonumeric(*path[0]);
    //increment path pointer now to skip the drive number Ex:0:/
    *path+=3;
    return drv_no;
}

static struct path_root *create_root(int drv_no)
{
    struct path_root *path_r = kernel_zalloc(sizeof(struct path_root));
    path_r->drv_no = drv_no;
    path_r->first = 0;
    return path_r;
}

static const char* get_path_part(const char **path)
{
    char *res_path_part = kernel_zalloc(MAX_FILEPATH);
    int  i = 0;
    while(**path != '/' && **path != '\0')
    {
        res_path_part[i] = **path;
        *path+=1;
        i++;
    }
    if(**path == '/')
    {
        *path+=1;
    }
    if(i == 0)
    {
        kernel_free(res_path_part);
        res_path_part = 0;
    }
    return res_path_part;
}

struct path_part *parse_path_part(struct path_part* last_part,const char** path)
{
    const char* path_str = get_path_part(path);
    if(!path_str)
    {
        return 0;
    }
    struct path_part *part = kernel_zalloc(sizeof(struct path_part));
    part->part = path_str;
    part->next = 0x0;
    if(last_part)
    {
        last_part->next = part;
    }
    return part;
}

void path_free(struct path_root *root)
{
    struct path_part* part = root->first;
    while(part)
    {
        struct path_part *next_part = part->next;
        kernel_free((void*)part->part);
        kernel_free((void*)part);
        part = next_part;
    }
    kernel_free(root);
}

struct path_root *path_parse(const char* path, const char* curr_dir)
{
    const char *temp_path = path;
    struct path_root* path_root = 0;

    if(strlen(path) > MAX_FILEPATH)
    {
        return 0;
    }
    int drv_no = get_drv_by_path(&temp_path);
    if(drv_no < 0)
    {
        return 0;
    }
    path_root = create_root(drv_no);
    struct path_part* first_part = parse_path_part(NULL,&temp_path);
    if(!first_part)
    {
        return 0;
    }
    path_root->first = first_part;
    struct path_part *part = parse_path_part(first_part,&temp_path);
    while(part)
    {
        part = parse_path_part(part,&temp_path);
    }
    return path_root;
}