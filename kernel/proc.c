#include <sys/const.h>
#include <sys/proc.h>
#include <sys/protect.h>
#include <sys/task.h>
#include <string.h>
#include <kio.h>
#include "global.h"
#include "misc.h"


#define UPDATE_LDT_ACCB(ldt, mask, attr) \
    (ldt).access_byte = ((ldt).access_byte & ~(mask)) | (attr)



void setup_proc_tbl()
{
    uint32_t stacktop = (uint32_t)task_stackbase + TASK_STACK_SIZE;

    for (int i = 0; i < NUM_TASK; ++i)
    {
        proc_tbl[i].regs.cs = LDT_SELECTOR_CODE | SELECTOR_LDT | tasks[i].rpl;
        proc_tbl[i].regs.ds = LDT_SELECTOR_DATA | SELECTOR_LDT | tasks[i].rpl;
        proc_tbl[i].regs.es = LDT_SELECTOR_DATA | SELECTOR_LDT | tasks[i].rpl;
        proc_tbl[i].regs.fs = LDT_SELECTOR_DATA | SELECTOR_LDT | tasks[i].rpl;
        proc_tbl[i].regs.gs = LDT_SELECTOR_DATA | SELECTOR_LDT | tasks[i].rpl;
        proc_tbl[i].regs.ss = LDT_SELECTOR_DATA | SELECTOR_LDT | tasks[i].rpl;
        
        proc_tbl[i].regs.eip = tasks[i].ptr;
        proc_tbl[i].regs.esp = stacktop;
        proc_tbl[i].regs.eflags = TASK_EFLAGS;

        memcpy(
            proc_tbl[i].ldts,
            &gdts[GDT_CODE_IDX],
            sizeof(struct gdt)
        );
        UPDATE_LDT_ACCB(proc_tbl[i].ldts[0], GDT_DPL_MASK, tasks[i].dpl);
        memcpy(
            proc_tbl[i].ldts + 1,
            &gdts[GDT_DATA_IDX],
            sizeof(struct gdt)
        );
        UPDATE_LDT_ACCB(proc_tbl[i].ldts[1], GDT_DPL_MASK, tasks[i].dpl);
        proc_tbl[i].priority = 5;
        proc_tbl[i].ticks = proc_tbl[i].priority;
        proc_tbl[i].pid = i;
        proc_tbl[i].tty = 0; // not used yet
        proc_tbl[i].ldt_selector = GDT_SELECTOR_LDT(i);
        strcpy(proc_tbl[i].name, tasks[i].name);

        stacktop -= tasks[i].stack_size;
    }

    for (int i = NUM_TASK; i < NUM_UTASK + NUM_TASK; ++i)
    {
        proc_tbl[i].regs.cs =
            LDT_SELECTOR_CODE | SELECTOR_LDT | user_tasks[i - NUM_TASK].rpl;
        proc_tbl[i].regs.ds = 
            LDT_SELECTOR_DATA | SELECTOR_LDT | user_tasks[i - NUM_TASK].rpl;
        proc_tbl[i].regs.es = 
            LDT_SELECTOR_DATA | SELECTOR_LDT | user_tasks[i - NUM_TASK].rpl;
        proc_tbl[i].regs.fs = 
            LDT_SELECTOR_DATA | SELECTOR_LDT | user_tasks[i - NUM_TASK].rpl;
        proc_tbl[i].regs.gs = 
            LDT_SELECTOR_DATA | SELECTOR_LDT | user_tasks[i - NUM_TASK].rpl;
        proc_tbl[i].regs.ss = 
            LDT_SELECTOR_DATA | SELECTOR_LDT | user_tasks[i - NUM_TASK].rpl;

        proc_tbl[i].regs.eip = user_tasks[i - NUM_TASK].ptr;
        proc_tbl[i].regs.esp = stacktop;
        proc_tbl[i].regs.eflags = UTASK_EFLAGS;

        memcpy(
            proc_tbl[i].ldts,
            &gdts[GDT_CODE_IDX],
            sizeof(struct gdt)
        );
        UPDATE_LDT_ACCB(proc_tbl[i].ldts[0], GDT_DPL_MASK, user_tasks[i - NUM_TASK].dpl);
        memcpy(
            proc_tbl[i].ldts + 1,
            &gdts[GDT_DATA_IDX],
            sizeof(struct gdt)
        );
        UPDATE_LDT_ACCB(proc_tbl[i].ldts[1], GDT_DPL_MASK, user_tasks[i - NUM_TASK].dpl);
        proc_tbl[i].priority = 5;
        proc_tbl[i].ticks = proc_tbl[i].priority;
        proc_tbl[i].pid = i;
        proc_tbl[i].tty = 0; // not used yet
        proc_tbl[i].ldt_selector = GDT_SELECTOR_LDT(i);
        strcpy(proc_tbl[i].name, user_tasks[i - NUM_TASK].name);

        stacktop -= user_tasks[i - NUM_TASK].stack_size;        
    }
}


void schedule()
{
    int greatest = 0;
    while (!greatest)
    {
        for (int i = 0; i < NUM_PROC; ++i)
        {
            if(greatest < proc_tbl[i].ticks)
            {
                curr_proc = &proc_tbl[i];
                greatest = curr_proc->ticks;
            }
        }

        if (!greatest)
        {
            for (int i = 0; i < NUM_PROC; ++i)
            {
                proc_tbl[i].ticks = proc_tbl[i].priority;
            }
        }
    }
}
