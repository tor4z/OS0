#include <sys/const.h>
#include <sys/protect.h>
#include <sys/proc.h>
#include <sys/task.h>
#include <type.h>
#include <kio.h>
#include "global.h"
#include "int_handler.h"
#include "kinit.h"
#include "syscall_impl.h"
#include "8259a.h"


static void setup_tss();
static void setup_paging();
static void setup_gdts();
static void setup_idts();
static void setup_irq_tbl();
static void setup_syscall_tbl();


void kinit()
{
    setup_gdts();
    setup_gdtr();
    setup_idtr();
    setup_tss();
    setup_irq_tbl();
    setup_syscall_tbl();

    __asm__ __volatile__(
        "lgdt (%0)"
        : /* no output */
        : "r"(&gdt_ptr)
        :
    );

    __asm__ __volatile__(
        "ltr %%ax"
        : /* no output */
        : "a"(GDT_SELECTOR_TSS)
        :
    );

    __asm__ __volatile__(
        "lidt (%0)"
        : /* no output */
        : "r"(&idt_ptr)
        :
    );

    setup_paging();
    setup_idts();

    // init global variables
    kreenter = 0;
}


static void setup_syscall_tbl()
{
    system_call_tbl[SYSCALL_GET_TICKS] = sys_get_ticks;
}


static void setup_irq_tbl()
{
    for (int i = 0; i< NUM_IRQ; ++i)
    {
        irq_tbl[i] = phony_handler;
    }
}


static void setup_tss()
{
    init_tss(&tss, 4096, GDT_SELECTOR_CODE);
    tss.ss0 = GDT_SELECTOR_DATA;
}


void setup_paging()
{
    uint32_t pde0 = PAGE_TBL_BASE | PG_P | PG_RW_RW | PG_US_U;
    uint32_t pte0 = PG_P | PG_RW_RW | PG_US_U;

    __asm__ __volatile__(
        ".set_pde:\n\t"
        "stosl\n\t"
        "addl $4096, %%eax\n\t"
        "loop .set_pde"
        : /* no output */
        : "a"(pde0), "D"(PAGE_DIR_BASE), "c"(NUM_PDE)
        : "memory"
    );

    __asm__ __volatile__(
        ".set_pte:\n\t"
        "stosl\n\t"
        "addl $4096, %%eax\n\t"
        "loop .set_pte\n\t"
        : /* no output */
        : "a"(pte0), "D"(PAGE_TBL_BASE), "c"(NUM_PTE)
        : "memory"
    );

    __asm__ __volatile__(
        "movl %%eax, %%cr3\n\t"
        "movl %%cr0, %%eax\n\t"
        "orl $0x80000000, %%eax\n\t"
        "movl %%eax, %%cr0"
        : /* no output */
        : "a"(PAGE_DIR_BASE)
        :
    );
}


void setup_gdts()
{
    init_gdt(
        &gdts[GDT_NULL_IDX], 0, 0, 0, 0
    );
    init_gdt(
        &gdts[GDT_CODE_IDX], 0, 0xfffff,
        GDT_PRES | GDT_NOTSYS | GDT_EXE |
            GDT_RW,
        GDT_PG | GDT_32BIT
    );
    init_gdt(
        &gdts[GDT_DATA_IDX], 0, 0xfffff,
        GDT_PRES | GDT_NOTSYS | GDT_RW,
        GDT_PG | GDT_32BIT
    );
    init_gdt(
        &gdts[GDT_TSS_IDX], (uint32_t)&tss, sizeof(struct tss),
        GDT_PRES | GDT_EXE | GDT_ACC,
        GDT_PG | GDT_32BIT
    );

    for (int i = 0; i < (NUM_PROC); ++i)
    {
        init_gdt(
            &gdts[GDT_LDT_IDX(i)], (uint32_t)proc_tbl[i].ldts,
            LDT_SIZE * NUM_LDT, GDT_RW | GDT_PRES,
            0
        );
    }
}


void setup_idts()
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
        &idts[INT_VEC_IRQ(0)], (uint32_t)irq0_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(1)], (uint32_t)irq1_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(2)], (uint32_t)irq2_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(3)], (uint32_t)irq3_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(4)], (uint32_t)irq4_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(5)], (uint32_t)irq5_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(6)], (uint32_t)irq6_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(7)], (uint32_t)irq7_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(8)], (uint32_t)irq8_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(9)], (uint32_t)irq9_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(10)], (uint32_t)irq10_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(11)], (uint32_t)irq11_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(12)], (uint32_t)irq12_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(13)], (uint32_t)irq13_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(14)], (uint32_t)irq14_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );
    init_idt(
        &idts[INT_VEC_IRQ(15)], (uint32_t)irq15_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL0 | INT_GATE_INT32
    );

    init_idt(
        &idts[INT_VEC_SYSCALL], (uint32_t)syscall_handler, GDT_SELECTOR_CODE,
        INT_GATE_P | INT_GATE_PL3 | INT_GATE_INT32
    );
}
