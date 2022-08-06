#include "int_handler.h"
#include <kernel/const.h>
#include <kernel/type.h>
#include <kernel/kio.h>
#include <kernel/protect.h>
#include <kernel/global.h>


static void setup_8259a();


void init_idts()
{
    setup_8259a();

    // setup default handler
    for (int i = 0; i < NUM_IDT; ++i)
    {
        init_idt(
            &idts[i], phony_handler, GDT_SELECTOR_CODE,
            INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
        );        
    }

    init_idt(
        &idts[INT_VEC_DE], de_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_DB], db_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_NMI], nmi_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_BP], bp_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_OF], of_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_BR], br_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_UD], ud_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_NM], nm_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_DF], df_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_CPS], cps_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_TS], ts_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_NP], np_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_SS], ss_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_GP], gp_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_PF], pf_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_NOTUSED], notused_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_MF], mf_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_AC], ac_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_MC], mc_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_XF], xf_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );

    init_idt(
        &idts[INT_VEC_IRQ0], irq0_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ1], irq1_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ2], irq2_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ3], irq3_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ4], irq4_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ5], irq5_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ6], irq6_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ7], irq7_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ8], irq8_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ9], irq9_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ10], irq10_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ11], irq11_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ12], irq12_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ13], irq13_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ14], irq14_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ15], irq15_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );

    init_idt(
        &idts[INT_VEC_SYSCALL], syscall_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL3 | INT_GATE_INT32
    );
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
