#include <kernel/const.h>
#include <kernel/type.h>
#include <kernel/kio.h>
#include <kernel/protect.h>


static void setup_8259a();


void init_idts()
{
    setup_8259a();

    init_idt(struct idt *idt_ptr, uint32_t offset, uint16_t selector, uint8_t attr)
}


static void setup_8259a()
{
    // ICW1
    // ICW4 needed
    // D4 = 1
    out_byte(MASTER_8259A_P0, 0x11);
    out_byte(SLAVE_8259A_P0, 0x11);

    // ICW2
    // master: vector addres 0x20
    // slave: vector addres 0x28
    out_byte(MASTER_8259A_P1, IRQ_MASTER_VEC);
    out_byte(SLAVE_8259A_P1, IRQ_SLAVE_VEC);

    // ICW3
    // master: D2 has slave
    // slave: set slave ID to 2
    out_byte(MASTER_8259A_P1, 0x04);
    out_byte(SLAVE_8259A_P1, 0x02);

    // ICW4
    // set 8086/8088 mode
    // normal EOI
    // no buffer mode
    out_byte(MASTER_8259A_P1, 0x01);
    out_byte(SLAVE_8259A_P1, 0x01);

    // OCW1
    out_byte(MASTER_8259A_P1, 0xff);
    out_byte(SLAVE_8259A_P1, 0xff);
}
