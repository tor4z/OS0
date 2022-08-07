#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <kernel/const.h>
#include <kernel/protect.h>
#include <kernel/type.h>


#ifdef IN_GLOBAL_C
    #define AUTO_EXTERN
#else
    #define AUTO_EXTERN extern
#endif // IN_GLOBAL


AUTO_EXTERN struct gdt gdts[NUM_GDT];
AUTO_EXTERN struct gdtr gdt_ptr;
AUTO_EXTERN struct tss tss;
AUTO_EXTERN struct idt idts[NUM_IDT];
AUTO_EXTERN struct idtr idt_ptr;

#undef AUTO_EXTERN
#endif // GLOBAL_H_
