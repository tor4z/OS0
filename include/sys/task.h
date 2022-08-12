#ifndef TASK_H_
#define TASK_H_

#include <sys/const.h>
#include <sys/protect.h>
#include <type.h>


#define NAME_SIZE 32        // task name size
#define STACK_SIZE 128      // stack size

#define DPL0 GDT_DPL0
#define DPL1 GDT_DPL1
#define DPL2 GDT_DPL2
#define DPL3 GDT_DPL3

#define RPL0 SELECTOR_RPL0
#define RPL1 SELECTOR_RPL1
#define RPL2 SELECTOR_RPL2
#define RPL3 SELECTOR_RPL3

#define N_TASK 1        // number of system task
#define N_UTASK 2       // number of user task

// bit 2 is always 1
#define TASK_EFLAGS  0x1202       // IF = 1, IOPL = 1
#define UTASK_EFLAGS 0x0202       // IF = 1


struct task
{
    uint32_t ptr;
    uint32_t stack_size;
    uint8_t dpl;
    uint8_t rpl;
    char name[NAME_SIZE];
};


void setup_tasks();
void setup_utasks();


#endif // TASK_H_
