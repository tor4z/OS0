#ifndef KIO_H_
#define KIO_H_

#include <kernel/type.h>


#define EOF -1


int32_t putchar(char c);
int32_t puts(const char *str);

// IO manipulation
void out_byte(uint8_t port, uint8_t data);
uint8_t in_byte(uint8_t port);

#endif // KIO_H_
