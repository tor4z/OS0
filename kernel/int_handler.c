#include <kernel/kio.h>
#include <kernel/const.h>
#include <kernel/type.h>
#include <klib.h>
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
    kputs("Phony interrupt handler");
}


void de_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_DE, -1, eip, cs, eflags);
}


void db_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_DB, -1, eip, cs, eflags);
}


void nmi_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_NMI, -1, eip, cs, eflags);
}


void bp_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_BP, -1, eip, cs, eflags);
}


void of_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_OF, -1, eip, cs, eflags);
}


void br_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_BR, -1, eip, cs, eflags);
}


void ud_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_UD, -1, eip, cs, eflags);
}


void nm_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_NM, -1, eip, cs, eflags);
}


void df_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_DF, err_code, eip, cs, eflags);
}


void cps_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_CPS, -1, eip, cs, eflags);
}


void ts_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_TS, err_code, eip, cs, eflags);
}


void np_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_NP, err_code, eip, cs, eflags);
}


void ss_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_SS, err_code, eip, cs, eflags);
}


void gp_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_GP, err_code, eip, cs, eflags);
}


void pf_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_PF, err_code, eip, cs, eflags);
}


void notused_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_NOTUSED, -1, eip, cs, eflags);
}


void mf_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_MF, -1, eip, cs, eflags);
}


void ac_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_AC, err_code, eip, cs, eflags);
}


void mc_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_MC, -1, eip, cs, eflags);
}


void xf_handler(uint32_t eip, uint32_t cs, uint32_t eflags)
{
    exception_handler(INT_VEC_XF, -1, eip, cs, eflags);
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

    kputs(exp_msg[vec_no]);
}
