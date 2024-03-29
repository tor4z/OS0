#include <sys/const.h>
#include <sys/clock.h>
#include <sys/proc.h>
#include <klib.h>
#include <kio.h>
#include "8259a.h"
#include "global.h"
#include "int_handler.h"



void clock_handler()
{
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
