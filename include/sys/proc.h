#ifndef PROC_H_
#define PROC_H_

#include <sys/const.h>
#include <sys/protect.h>
#include <type.h>


#define NAME_SIZE 32
#define NUM_LDT 2

#define LDT_CODE_IDX 0
#define LDT_DATA_IDX 1
#define LDT_SELECTOR_CODE (LDT_CODE_IDX << 3)
#define LDT_SELECTOR_DATA (LDT_DATA_IDX << 3)


#define PROC_OFF_GS        0
#define PROC_OFF_FS        (PROC_OFF_GS + 4)
#define PROC_OFF_ES        (PROC_OFF_FS + 4)
#define PROC_OFF_DS        (PROC_OFF_ES + 4)
#define PROC_OFF_EDI       (PROC_OFF_DS + 4)
#define PROC_OFF_ESI       (PROC_OFF_EDI + 4)
#define PROC_OFF_EBP       (PROC_OFF_ESI + 4)
#define PROC_OFF_KESP      (PROC_OFF_EBP + 4)
#define PROC_OFF_EBX       (PROC_OFF_KESP + 4)
#define PROC_OFF_EDX       (PROC_OFF_EBX + 4)
#define PROC_OFF_ECX       (PROC_OFF_EDX + 4)
#define PROC_OFF_EAX       (PROC_OFF_ECX + 4)
#define PROC_OFF_RET       (PROC_OFF_EAX + 4)
#define PROC_OFF_EIP       (PROC_OFF_RET + 4)
#define PROC_OFF_CS        (PROC_OFF_EIP + 4)
#define PROC_OFF_EFLAGS    (PROC_OFF_CS + 4)
#define PROC_OFF_ESP       (PROC_OFF_EFLAGS + 4)
#define PROC_OFF_SS        (PROC_OFF_ESP + 4)
#define PROC_OFF_LDT       (PROC_OFF_SS + 4)


struct frame
{
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    uint32_t edi;
    uint32_t esi;
    uint32_t ebp;
    uint32_t kesp;
    uint32_t ebx;
    uint32_t edx;
    uint32_t ecx;
    uint32_t eax;
    uint32_t ret;       // return addr for save
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
};


struct proc
{
    struct frame regs;
    struct ldt ldts[NUM_LDT];
    int32_t ticks;
    int32_t priority;
    uint32_t pid;
    int32_t tty;
    uint16_t ldt_selector;
    char name[NAME_SIZE];
};


void setup_proc_tbl();
void schedule();


#endif // PROC_H_
