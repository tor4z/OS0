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
            &idts[i], (uint32_t)phony_handler, GDT_SELECTOR_CODE,
            INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
        );        
    }

    init_idt(
        &idts[INT_VEC_DE], (uint32_t)de_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_DB], (uint32_t)db_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_NMI], (uint32_t)nmi_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_BP], (uint32_t)bp_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_OF], (uint32_t)of_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_BR], (uint32_t)br_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_UD], (uint32_t)ud_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_NM], (uint32_t)nm_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_DF], (uint32_t)df_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_CPS], (uint32_t)cps_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_TS], (uint32_t)ts_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_NP], (uint32_t)np_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_SS], (uint32_t)ss_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_GP], (uint32_t)gp_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_PF], (uint32_t)pf_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_NOTUSED], (uint32_t)notused_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_MF], (uint32_t)mf_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_AC], (uint32_t)ac_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_MC], (uint32_t)mc_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_XF], (uint32_t)xf_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );

    init_idt(
        &idts[INT_VEC_IRQ0], (uint32_t)irq0_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ1], (uint32_t)irq1_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ2], (uint32_t)irq2_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ3], (uint32_t)irq3_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ4], (uint32_t)irq4_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ5], (uint32_t)irq5_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ6], (uint32_t)irq6_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ7], (uint32_t)irq7_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ8], (uint32_t)irq8_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ9], (uint32_t)irq9_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ10], (uint32_t)irq10_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ11], (uint32_t)irq11_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ12], (uint32_t)irq12_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ13], (uint32_t)irq13_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ14], (uint32_t)irq14_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ15], (uint32_t)irq15_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );

    init_idt(
        &idts[INT_VEC_SYSCALL], (uint32_t)syscall_handler, GDT_SELECTOR_CODE,
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
