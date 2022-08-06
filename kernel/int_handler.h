#ifndef INT_HANDLER_H_
#define INT_HANDLER_H_


void phony_handler();

void de_handler();
void db_handler();
void nmi_handler();
void bp_handler();
void of_handler();
void br_handler();
void ud_handler();
void nm_handler();
void df_handler();
void cps_handler();
void ts_handler();
void np_handler();
void ss_handler();
void gp_handler();
void pf_handler();
void notused_handler();
void mf_handler();
void ac_handler();
void mc_handler();
void xf_handler();

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

void syscall_handler();


#endif // INT_HANDLER_H_