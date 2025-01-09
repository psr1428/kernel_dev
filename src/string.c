#include "string.h"

int strlen(const char *str)
{
    int str_len = 0;
    int i = 0;
    while(str[i] != '\0')
    {
        str_len++;
        i++;
    }
    return str_len;
}

int strnlen(const char *str,int max)
{
    int str_len = 0;
    for(str_len = 0; str_len < max; str_len++)
    {
        if(str[str_len] == '\0')
        {
            break;
        }
    }
    return str_len;
}

int tonumeric(char c)
{
    return c-48;
}

bool isDigit(char c)
{
    return (c >=48 && c <= 57);
}

int memcmp(void *str1,void *str2,int cnt)
{
    char *c1 = str1;
    char *c2 = str2;
    while(cnt--)
    {
        if(*c1++ != *c2++)
        {
            return c1[-1] < c2[-1] ? -1: 1;
        }
    }
    return 0;
}
