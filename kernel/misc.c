#include "misc.h"


void pause()
{
    __asm__ __volatile__("jmp .");
}
