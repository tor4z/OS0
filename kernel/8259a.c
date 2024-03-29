#include <sys/const.h>
#include <kio.h>
#include "8259a.h"
#include "restart.h"
#include "global.h"


#define MASTER_CMD      0x20
#define SLAVE_CMD       0xa0
#define MASTER_DATA     0x21
#define SLAVE_DATA      0xa1
#define EOI             0x20
#define MASTER_IRQ_LIM  7


void setup_8259a()
{
    // ICW1
    // ICW4 needed
    // D4 = 1
    out_byte(MASTER_CMD, 0x11);
    out_byte(SLAVE_CMD, 0x11);

    // ICW2
    // master: vector addres 0x20
    // slave: vector addres 0x28
    out_byte(MASTER_DATA, IRQ_MASTER_VEC);
    out_byte(SLAVE_DATA, IRQ_SLAVE_VEC);

    // ICW3
    // master: D2 has slave
    // slave: set slave ID to 2
    out_byte(MASTER_DATA, 0x04);
    out_byte(SLAVE_DATA, 0x02);

    // ICW4
    // set 8086/8088 mode
    // normal EOI
    // no buffer mode
    out_byte(MASTER_DATA, 0x01);
    out_byte(SLAVE_DATA, 0x01);

    // OCW1
    out_byte(MASTER_DATA, 0xff);
    out_byte(SLAVE_DATA, 0xff);
}


void master_int(int id)
{
    // mask this type interrupt
    disable_irq(id);
    // set end of intterrupt
    out_byte(MASTER_CMD, EOI);

    ENABLE_INT();
    irq_tbl[id]();
    DISABLE_INT();

    // unmask this type interrupt
    enable_irq(id);
}


void slave_int(int id)
{
    // mask this type interrupt
    disable_irq(id);
    // set end of intterrupt
    out_byte(MASTER_CMD, EOI);
    out_byte(SLAVE_CMD, EOI);

    ENABLE_INT();
    irq_tbl[id]();
    DISABLE_INT();

    // unmask this type interrupt
    enable_irq(id);
}


void disable_irq(int id)
{
    int port;
    if(id > MASTER_IRQ_LIM)
        port = SLAVE_DATA;
    else
        port = MASTER_DATA;

    uint8_t mask = (1 << id);
    uint8_t data = in_byte(port);
    data |= mask;
    out_byte(port, data);
}


void enable_irq(int id)
{
    int port;
    if(id > MASTER_IRQ_LIM)
        port = SLAVE_DATA;
    else
        port = MASTER_DATA;

    uint8_t mask = ~(1 << id);
    uint8_t data = in_byte(port);
    data &= mask;
    out_byte(port, data);
}
