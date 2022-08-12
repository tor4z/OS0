#include "misc.h"
#include "global.h"


static uint32_t segbase(uint16_t selector);


void pause()
{
    __asm__ __volatile__("jmp .");
}


uint32_t vaddr2phy(uint16_t selector, uint32_t vaddr)
{
    return segbase(selector) + vaddr;
}


static uint32_t segbase(uint16_t selector)
{
    uint32_t base = 0;
    struct gdt *gdt = gdts + (selector >> 3);

    base |= gdt->base0_15 & 0xffff;
    base |= (gdt->base16_23 << 16) & 0xff0000;
    base |= (gdt->base24_31 << 24) & 0xff000000;
    
    return base;
}
