#include <sys/vga.h>
#include <sys/proc.h>
#include <sys/task.h>
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

    restart();
    // We will never reach here.
    return 0;
}
