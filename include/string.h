#ifndef STRING_H_
#define STRING_H_

#include <type.h>


void *memcpy(void *dest, const void * src, size_t n);
void *memset(void *ptr, int x, size_t n);

char* strcpy(char* dest, const char* src);
size_t strlen(const char *str);

#endif // STRING_H_
