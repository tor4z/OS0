#ifndef CONST_H_
#define CONST_H_

#define KER_STACK_SIZE 4096
#define TASK_STACK_SIZE 4096

#define NUM_GDT 128
#define NUM_IDT 256

#define NUM_PDE 1024
#define NUM_PTE (1024 * NUM_PDE)

#define MASTER_8259A_P0 0x20
#define SLAVE_8259A_P0  0xa0
#define MASTER_8259A_P1 0x21
#define SLAVE_8259A_P1  0xa1

#define IRQ_MASTER_VEC  0x20
#define IRQ_SLAVE_VEC   0x28

//         NAME         CODE       TYPE      ERR_CODE       SOURCE
#define INT_VEC_DE      0x00    /* Fault        N     DIV/IDIV intstruct */
#define INT_VEC_DB      0x01    /* Fault/Trap   N     Any code/data access */
#define INT_VEC_NMI     0x02    /* Interrupt    N     Non-maskable interrupt */
#define INT_VEC_BP      0x03    /* Trap         N     INT 3 intstruct */
#define INT_VEC_OF      0x04    /* Trap         N     INTO intstruct */
#define INT_VEC_BR      0x05    /* Fault        N     BOUND intstruct */
#define INT_VEC_UD      0x06    /* Fault        N     UD2/Invalid intstruct */
#define INT_VEC_NM      0x07    /* Fault        N     FLOAT/WAIT/FWAIT intstruct */
#define INT_VEC_DF      0x08    /* Abort        Y(0)  NMI/INTR intstruct */
#define INT_VEC_CPS     0x09    /* Fault        N     FLOAT intstruct */
#define INT_VEC_TS      0x0a    /* Fault        Y     Task change/ TSS access */
#define INT_VEC_NP      0x0b    /* Fault        Y     Load seg register / access system seg */
#define INT_VEC_SS      0x0c    /* Fault        Y     Heap operation / load ss */
#define INT_VEC_GP      0x0d    /* Fault        Y     Memory / other protection check */
#define INT_VEC_PF      0x0e    /* Fault        Y     Memory access */
#define INT_VEC_NOTUSED 0x0f    /*   -          -     - */
#define INT_VEC_MF      0x10    /* Fault        N     x87FPU FLOAT instruct / WAIT / FWAIT */
#define INT_VEC_AC      0x11    /* Fault        Y(0)  Data access in memory (since 486) */
#define INT_VEC_MC      0x12    /* Abort        N     Machine check (Since Pentium) */
#define INT_VEC_XF      0x13    /* Fault        N     SSE / SSE2 (Since Pentium III) */
// IRQ
#define INT_VEC_IRQ0    0x20
#define INT_VEC_IRQ1    0x21
#define INT_VEC_IRQ2    0x22
#define INT_VEC_IRQ3    0x23
#define INT_VEC_IRQ4    0x24
#define INT_VEC_IRQ5    0x25
#define INT_VEC_IRQ6    0x26
#define INT_VEC_IRQ7    0x27
#define INT_VEC_IRQ8    0x28
#define INT_VEC_IRQ9    0x29
#define INT_VEC_IRQ10   0x2a
#define INT_VEC_IRQ11   0x2b
#define INT_VEC_IRQ12   0x2c
#define INT_VEC_IRQ13   0x2d
#define INT_VEC_IRQ14   0x2e
#define INT_VEC_IRQ15   0x2f
// sys
#define INT_VEC_SYSCALL 0x88

#endif // CONST_H_
