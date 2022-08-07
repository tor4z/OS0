#ifndef STRING_H_
#define STRING_H_

#include <kernel/type.h>


void *memcpy(void *dest, const void * src, size_t n);
void *memset(void *ptr, int x, size_t n);


#endif // STRING_H_