#ifndef KIO_H_
#define KIO_H_

#include <kernel/type.h>


#define EOF -1


int32_t kputchar(char c);
int32_t kputs(const char *str);
int32_t kprint(const char *str);

// IO manipulation
void out_byte(uint16_t port, uint8_t data);
uint8_t in_byte(uint16_t port);

#endif // KIO_H_
