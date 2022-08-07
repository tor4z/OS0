#include <kernel/kio.h>
#include "int_handler.h"


static void exception_handler(
    uint32_t vec_no, int32_t err_code,
    uint32_t eip, uint32_t cs, uint32_t eflags
);


static char exp_msg[][64] =
{
    "DE: Devide Error.",
    "DB: code / data access",
    "NMI: non-maskable interrupt",
    "BP: break point",
    "OF: overflow",
    "BR: BOUND range exceeded ",
    "UD: invalid opcode",
    "NM: no math coprocessor",
    "DF: double fault",
    "CPS: coprocessor segment overrun",
    "TS: invalid TSS",
    "NP: segment not present",
    "SS: stack segment fault",
    "GP: general protection",
    "PF: page fault",
    "NOTUSED: ",
    "MF: x87 FPU floating-point error",
    "AC: alignment check",
    "MC: mechine check",
    "XF: SIMD floating-point exception"
};


void phony_handler()
{
    puts("Phony interrupt handler");
}


void de_handler()
{

}


void db_handler()
{

}


void nmi_handler()
{

}


void bp_handler()
{

}


void of_handler()
{

}


void br_handler()
{

}


void ud_handler()
{

}


void nm_handler()
{

}


void df_handler()
{

}


void cps_handler()
{

}


void ts_handler()
{

}


void np_handler()
{

}


void ss_handler()
{

}


void gp_handler()
{

}


void pf_handler()
{

}


void notused_handler()
{

}


void mf_handler()
{

}


void ac_handler()
{

}


void mc_handler()
{

}


void xf_handler()
{

}


void irq0_handler()
{

}


void irq1_handler()
{

}


void irq2_handler()
{

}


void irq3_handler()
{

}


void irq4_handler()
{

}


void irq5_handler()
{

}


void irq6_handler()
{

}


void irq7_handler()
{

}


void irq8_handler()
{

}


void irq9_handler()
{

}


void irq10_handler()
{

}


void irq11_handler()
{

}


void irq12_handler()
{

}


void irq13_handler()
{

}


void irq14_handler()
{

}


void irq15_handler()
{

}


void syscall_handler()
{

}


static void exception_handler(
    uint32_t vec_no, int32_t err_code,
    uint32_t eip, uint32_t cs, uint32_t eflags
)
{

}
