#ifndef PROTECT_H_
#define GLOBAL_H_

#include <kernel/const.h>
#include <kernel/protect.h>


#ifdef IN_GLOBAL_C
#define AUTO_EXTERN
#else
#define AUTO_EXTERN extern
#endif // IN_GLOBAL


AUTO_EXTERN struct gdt gdt[NUM_GDT];


#endif // GLOBAL_H_
