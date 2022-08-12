#ifndef CONST_H_
#define CONST_H_

#define KER_STACK_SIZE 4096
#define TASK_STACK_SIZE 4096

#define NUM_GDT 128
#define NUM_IDT 256
#define NUM_LDT 2
#define GDT_SIZE 8
#define LDT_SIZE GDT_SIZE
#define IDT_SIZE GDT_SIZE

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


#define PROC_OFF_GS             0
#define PROC_OFF_FS             (PROC_OFF_GS + 4)
#define PROC_OFF_ES             (PROC_OFF_FS + 4)
#define PROC_OFF_DS             (PROC_OFF_ES + 4)
#define PROC_OFF_EDI            (PROC_OFF_DS + 4)
#define PROC_OFF_ESI            (PROC_OFF_EDI + 4)
#define PROC_OFF_EBP            (PROC_OFF_ESI + 4)
#define PROC_OFF_KESP           (PROC_OFF_EBP + 4)
#define PROC_OFF_EBX            (PROC_OFF_KESP + 4)
#define PROC_OFF_EDX            (PROC_OFF_EBX + 4)
#define PROC_OFF_ECX            (PROC_OFF_EDX + 4)
#define PROC_OFF_EAX            (PROC_OFF_ECX + 4)
#define PROC_OFF_RET            (PROC_OFF_EAX + 4)
#define PROC_OFF_EIP            (PROC_OFF_RET + 4)
#define PROC_OFF_CS             (PROC_OFF_EIP + 4)
#define PROC_OFF_EFLAGS         (PROC_OFF_CS + 4)
#define PROC_OFF_ESP            (PROC_OFF_EFLAGS + 4)
#define PROC_OFF_SS             (PROC_OFF_ESP + 4)
#define PROC_OFF_LDT            (PROC_OFF_SS + 4)
#define PROC_OFF_TICKS          (PROC_OFF_LDT + (NUM_LDT * LDT_SIZE))
#define PROC_OFF_PRIORITY       (PROC_OFF_TICKS + 4)
#define PROC_OFF_PID            (PROC_OFF_PRIORITY + 4)
#define PROC_OFF_TTY            (PROC_OFF_PID + 4)
#define PROC_OFF_LDT_SELECTOR   (PROC_OFF_TTY + 4)
#define PROC_OFF_NAME           (PROC_OFF_LDT_SELECTOR + 2)


#define TSS_OFF_BL      0
#define TSS_OFF_ESP0    (TSS_OFF_BL + 4)
#define TSS_OFF_SS0     (TSS_OFF_ESP0 + 4)
#define TSS_OFF_ESP1    (TSS_OFF_SS0 + 4)
#define TSS_OFF_SS1     (TSS_OFF_ESP1 + 4)
#define TSS_OFF_ESP2    (TSS_OFF_SS1 + 4)
#define TSS_OFF_SS2     (TSS_OFF_ESP2 + 4)
#define TSS_OFF_CR3     (TSS_OFF_SS2 + 4)
#define TSS_OFF_EIP     (TSS_OFF_CR3 + 4)
#define TSS_OFF_EFLAGS  (TSS_OFF_EIP + 4)
#define TSS_OFF_EAX     (TSS_OFF_EFLAGS + 4)
#define TSS_OFF_ECX     (TSS_OFF_EAX + 4)
#define TSS_OFF_EDX     (TSS_OFF_ECX + 4)
#define TSS_OFF_EBX     (TSS_OFF_EDX + 4)
#define TSS_OFF_EBP     (TSS_OFF_EBX + 4)
#define TSS_OFF_ESI     (TSS_OFF_EBP + 4)
#define TSS_OFF_EDI     (TSS_OFF_ESI + 4)
#define TSS_OFF_ES      (TSS_OFF_EDI + 4)
#define TSS_OFF_CS      (TSS_OFF_ES + 4)
#define TSS_OFF_SS      (TSS_OFF_CS + 4)
#define TSS_OFF_DS      (TSS_OFF_SS + 4)
#define TSS_OFF_FS      (TSS_OFF_DS + 4)
#define TSS_OFF_GS      (TSS_OFF_FS + 4)
#define TSS_OFF_LDTR    (TSS_OFF_GS + 4)
#define TSS_OFF_TRAP    (TSS_OFF_LDTR + 4)
#define TSS_OFF_IOBASE  (TSS_OFF_TRAP + 2)

#endif // CONST_H_
