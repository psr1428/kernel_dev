#ifndef PATHPARSER_H
#define PATHPARSER_H

#define MAX_FILEPATH 128

struct path_root
{
    int drv_no;
    struct path_part* first;
};

struct path_part
{
    const char *part;
    struct path_part *next;
};
struct path_root *path_parse(const char* path, const char* curr_dir);
void path_free(struct path_root *root);

#endif