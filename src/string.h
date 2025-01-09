#ifndef STRING_H
#define STRING_H

#include "config.h"
#include  <stdbool.h>

int strlen(const char *str);
bool isDigit(char c);
int tonumeric(char c);
int strnlen(const char *str,int max);
int memcmp(void *str1,void *str2,int cnt);
#endif