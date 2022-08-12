#include <sys/const.h>
#include <sys/clock.h>
#include <sys/proc.h>
#include "8259a.h"
#include "global.h"
#include "int_handler.h"
#include "klib.h"
#include <kio.h>


void clock_handler()
{
    char buff[16];
    kputs("clock");
    kprint("curr_proc->ticks: ");
    kputs(itoa(curr_proc->ticks, buff, 10));
    kprint("kreenter: ");
    kputs(itoa(kreenter, buff, 10));

    --curr_proc->ticks;

    if (kreenter)   // in kernel
        return;

    if (curr_proc->ticks > 0)
        return;

    schedule();
}


void init_clock()
{
    // TODO init 8253
    set_irq_handler(IRQ_CLOCK, clock_handler);
    enable_irq(IRQ_CLOCK);
}
