#include <sys/vga.h>
#include <sys/proc.h>
#include <sys/task.h>
#include <sys/clock.h>
#include <kio.h>
#include <klib.h>
#include "misc.h"
#include "global.h"
#include "restart.h"


int kmain()
{
    clean_screen();
    kputs("Hello world");

    setup_tasks();
    setup_utasks();
    setup_proc_tbl();
    curr_proc = proc_tbl;
    init_clock();

    char buff[16];
    kputs(itoa((uint32_t)curr_proc, buff, 16));
    
    kprint("kreenter: ");
    kputs(itoa(kreenter, buff, 10));

    restart();
    // We will never reach here.
    return 0;
}
