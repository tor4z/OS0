#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <kernel/const.h>
#include <kernel/protect.h>


#ifdef IN_GLOBAL_C
    #define AUTO_EXTERN
#else
    #define AUTO_EXTERN extern
#endif // IN_GLOBAL


AUTO_EXTERN struct gdt gdts[NUM_GDT];
AUTO_EXTERN struct gdtr gdt_ptr;

AUTO_EXTERN struct idt idts[NUM_IDT];


#endif // GLOBAL_H_
