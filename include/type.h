#ifndef TYPE_H_
#define TYPE_H_

typedef int int32_t;
typedef unsigned int uint32_t;

typedef short int16_t;
typedef unsigned short uint16_t;

typedef char int8_t;
typedef unsigned char uint8_t;

typedef unsigned long size_t;

typedef void (*irq_handler)();
typedef int (*system_call)();

#endif // TYPE_H_
