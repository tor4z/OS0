#include <sys/const.h>
#include <type.h>
#include <klib.h>
#include <kio.h>
#include "8259a.h"
#include "global.h"
#include "int_handler.h"
#include "restart.h"


#define GET_ARG0(arg)           \
    __asm__ __volatile__(       \
        "movl 4(%%ebp), %%eax"  \
        : "=a"(arg)             \
        :                       \
        :                       \
    )


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
    kputs("Phony interrupt handler");
}


void de_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_DE, 0, eip, cs, eflags);
}


void db_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_DB, 0, eip, cs, eflags);
}


void nmi_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_NMI, 0, eip, cs, eflags);
}


void bp_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_BP, 0, eip, cs, eflags);
}


void of_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_OF, 0, eip, cs, eflags);
}


void br_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_BR, 0, eip, cs, eflags);
}


void ud_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_UD, 0, eip, cs, eflags);
}


void nm_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_NM, 0, eip, cs, eflags);
}


void df_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    uint32_t err_code;
    GET_ARG0(err_code);
    exception_handler(INT_VEC_DF, err_code, eip, cs, eflags);
}


void cps_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_CPS, 0, eip, cs, eflags);
}


void ts_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    uint32_t err_code;
    GET_ARG0(err_code);
    exception_handler(INT_VEC_TS, err_code, eip, cs, eflags);
}


void np_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    uint32_t err_code;
    GET_ARG0(err_code);
    exception_handler(INT_VEC_NP, err_code, eip, cs, eflags);
}


void ss_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    uint32_t err_code;
    GET_ARG0(err_code);
    exception_handler(INT_VEC_SS, err_code, eip, cs, eflags);
}


void gp_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    uint32_t err_code;
    GET_ARG0(err_code);
    exception_handler(INT_VEC_GP, err_code, eip, cs, eflags);
}


void pf_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    uint32_t err_code;
    GET_ARG0(err_code);
    exception_handler(INT_VEC_PF, err_code, eip, cs, eflags);
}


void notused_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_NOTUSED, 0, eip, cs, eflags);
}


void mf_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_MF, 0, eip, cs, eflags);
}


void ac_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    uint32_t err_code;
    GET_ARG0(err_code);
    exception_handler(INT_VEC_AC, err_code, eip, cs, eflags);
}


void mc_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_MC, 0, eip, cs, eflags);
}


void xf_handler(uint32_t cs, uint32_t eflags)
{
    uint32_t eip;
    GET_ARG0(eip);
    exception_handler(INT_VEC_XF, 0, eip, cs, eflags);
}


// void irq0_handler()
// {
//     save();
//     master_int(0);
// }


void irq1_handler()
{
    save();
    master_int(1);
}


void irq2_handler()
{
    save();
    master_int(2);
}


void irq3_handler()
{
    save();
    master_int(3);
}


void irq4_handler()
{
    save();
    master_int(4);
}


void irq5_handler()
{
    save();
    master_int(5);
}


void irq6_handler()
{
    save();
    master_int(6);
}


void irq7_handler()
{
    save();
    master_int(7);
}


void irq8_handler()
{
    save();
    slave_int(8);
}


void irq9_handler()
{
    save();
    slave_int(9);
}


void irq10_handler()
{
    save();
    slave_int(10);
}


void irq11_handler()
{
    save();
    slave_int(11);
}


void irq12_handler()
{
    save();
    slave_int(12);
}


void irq13_handler()
{
    save();
    slave_int(13);
}


void irq14_handler()
{
    save();
    slave_int(14);
}


void irq15_handler()
{
    save();
    slave_int(15);
}


void set_irq_handler(int id, irq_handler handler)
{
    irq_tbl[id] = handler;
    disable_irq(id);
}


// void syscall_handler()
// {
//     save();
//     ENABLE_INT();
//     int val = system_call_tbl[0]();
//     __asm__ __volatile__(
//         ""
//         : /* no output */
//         :
//         :
//     );
//     DISABLE_INT();
// }


static void exception_handler(
    uint32_t vec_no, int32_t err_code,
    uint32_t eip, uint32_t cs, uint32_t eflags
)
{
    char buff[16];

    kprint("error code: ");
    kprint(itoa(err_code, buff, 16));
    kputchar('\n');

    kprint("eip: ");
    kprint(itoa(eip, buff, 16));
    kputchar('\n');

    kprint("cs: ");
    kprint(itoa(cs, buff, 16));
    kputchar('\n');

    kprint("eflags: ");
    kprint(itoa(eflags, buff, 16));
    kputchar('\n');

    kputs(exp_msg[vec_no]);
    
    __asm__ __volatile__(
        "hlt"
    );
}
