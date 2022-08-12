#include <sys/const.h>

    .extern curr_proc

    .section .text
    .global restart
    .global save

    .type restart, @function
restart:
    movl curr_proc, %esp
    lldt PROC_OFF_LDT_SELECTOR(%esp)
    leal PROC_OFF_SS(%esp), %eax
    movl $tss, %esi
    movl %eax, TSS_OFF_ESP0(%esi)
reenter:
    decl kreenter
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popal
    addl $4, %esp   /* skip ret */
    iret


    .type save, @function
save:
    ret
