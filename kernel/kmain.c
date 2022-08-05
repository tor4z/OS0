#include <kernel/kio.h>
#include "kernel/vga.h"
#include "misc.h"


int kmain()
{
    clean_screen();
    puts("Hello world");
    putchar('X');
    pause();
    return 0;
}
