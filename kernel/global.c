#include "kio.h"
#include "klib.h"
#define IN_GLOBAL_C

#include "global.h"


void inc_kreenter()
{
    char buff[16];
    kprint("inc kreenter: ");
    kputs(itoa(kreenter, buff, 10));
    // ++kreenter;
}


void dec_kreenter()
{
    char buff[16];
    kprint("dec kreenter: ");
    kputs(itoa(kreenter, buff, 10));
    // --kreenter;
}
