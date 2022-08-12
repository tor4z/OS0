#ifndef INT_HANDLER_H_
#define INT_HANDLER_H_

#include "sys/const.h"
#include <type.h>


void phony_handler();

void de_handler(uint32_t eip, uint32_t cs, uint32_t eflags);
void db_handler(uint32_t eip, uint32_t cs, uint32_t eflags);
void nmi_handler(uint32_t eip, uint32_t cs, uint32_t eflags);
void bp_handler(uint32_t eip, uint32_t cs, uint32_t eflags);
void of_handler(uint32_t eip, uint32_t cs, uint32_t eflags);
void br_handler(uint32_t eip, uint32_t cs, uint32_t eflags);
void ud_handler(uint32_t eip, uint32_t cs, uint32_t eflags);
void nm_handler(uint32_t eip, uint32_t cs, uint32_t eflags);
void df_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags);
void cps_handler(uint32_t eip, uint32_t cs, uint32_t eflags);
void ts_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags);
void np_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags);
void ss_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags);
void gp_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags);
void pf_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags);
void notused_handler(uint32_t eip, uint32_t cs, uint32_t eflags);
void mf_handler(uint32_t eip, uint32_t cs, uint32_t eflags);
void ac_handler(uint32_t err_code, uint32_t eip, uint32_t cs, uint32_t eflags);
void mc_handler(uint32_t eip, uint32_t cs, uint32_t eflags);
void xf_handler(uint32_t eip, uint32_t cs, uint32_t eflags);


void irq0_handler();
void irq1_handler();
void irq2_handler();
void irq3_handler();
void irq4_handler();
void irq5_handler();
void irq6_handler();
void irq7_handler();
void irq8_handler();
void irq9_handler();
void irq10_handler();
void irq11_handler();
void irq12_handler();
void irq13_handler();
void irq14_handler();
void irq15_handler();

void set_irq_handler(int id, irq_handler handler);

void syscall_handler();


#endif // INT_HANDLER_H_