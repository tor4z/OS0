#include <sys/vga.h>
#include <kio.h>
#include <klib.h>
#include "misc.h"
#include "global.h"
#include "restart.h"


int kmain()
{
    clean_screen();
    kputs("Hello world");

    restart();
    // We will never reach here.
    return 0;
}
