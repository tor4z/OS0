#include <kernel/kio.h>
#include <kernel/vga.h>
#include "misc.h"


int kmain()
{
    clean_screen();
    kputs("Hello world");
    kputchar('X');
    pause();
    return 0;
}
