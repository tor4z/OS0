#include <kernel/const.h>
#include <kernel/protect.h>
#include <kernel/global.h>
#include <kernel/type.h>


void init_gdt(
    struct gdt *gdt_ptr, uint32_t base, uint32_t limit,
    uint8_t access, uint8_t flags
)
{
    gdt_ptr->limit0_15 = limit & 0xffff;
    gdt_ptr->base0_15 = base & 0xffff;
    gdt_ptr->base16_23 = (base >> 16) & 0xff;
    gdt_ptr->access_byte = access & 0xff;
    gdt_ptr->limit16_19_flags = ((limit >> 16) & 0x0f) | (flags & 0xf0);
    gdt_ptr->base24_31 = (base >> 24) & 0xff;
}


void init_tss(struct tss *tss_ptr, uint32_t stack_top, uint32_t selector)
{
    tss_ptr->esp0 = stack_top;
    tss_ptr->ss0 = selector;
    tss_ptr->iobase = sizeof(struct tss);
}


void init_ldt(
    struct gdt *gdt_ptr, uint32_t base, uint32_t limit,
    uint8_t access, uint8_t flags
)
{
    init_gdt(gdt_ptr, base, limit, access, flags);
}


void init_idt(
    struct idt *idt_ptr, int_handler handler, uint16_t selector,
    uint8_t attr
)
{
    uint32_t offset = (uint32_t)handler;
    idt_ptr->offset0_15 = offset & 0xffff;
    idt_ptr->selector = selector;
    idt_ptr->zero = 0;
    idt_ptr->attr = attr;
    idt_ptr->offset16_31 = (offset >> 16) & 0xffff;
}


void setup_gdtr()
{
    gdt_ptr.size = NUM_GDT * GDT_SIZE - 1;
    gdt_ptr.base = gdts;
}