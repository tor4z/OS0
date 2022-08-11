#include <sys/kio.h>
#include <sys/vga.h>
#include "misc.h"
#include <klib.h>


int kmain()
{
    clean_screen();
    kputs("Hello world");
    kputchar('X');

    // __asm__ __volatile__(
    //     "int $0x88"
    // );
    int i = 1, j = 0;
    i = i / j;

    pause();
    return 0;
}
