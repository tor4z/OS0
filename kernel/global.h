#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <sys/const.h>
#include <sys/protect.h>
#include <sys/task.h>
#include <sys/proc.h>
#include <type.h>


#ifdef IN_GLOBAL_C
    #define EXTERN
#else
    #define EXTERN extern
#endif // IN_GLOBAL


EXTERN struct gdt gdts[NUM_GDT];
EXTERN struct gdtr gdt_ptr;
EXTERN struct tss tss;
EXTERN struct idt idts[NUM_IDT];
EXTERN struct idtr idt_ptr;


EXTERN int32_t kreenter;


EXTERN struct task tasks[N_TASK];
EXTERN struct task user_tasks[N_UTASK];
EXTERN struct proc proc_tbl[N_TASK + N_UTASK];


EXTERN char task_stackbase[TASK_STACK_SIZE];


extern struct proc *curr_proc;


#undef EXTERN
#endif // GLOBAL_H_
