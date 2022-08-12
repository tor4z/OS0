#include <sys/const.h>

    .extern curr_proc
    .extern ker_stackbase

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
restart_enter:
    decl kreenter
    popl %gs
    popl %fs
    popl %es
    popl %ds
    popal
    addl $4, %esp   /* skip the ret for save */
    iret


    .type save, @function
save:
    pushal
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    incl kreenter
    cmpl $0, kreenter
    je in_kernel

    movl PROC_OFF_RET(%esp), %esi
not_kernel:
    movl $ker_stackbase, %esp   // kernel stack top
    pushl restart
    jmp save_ret
in_kernel:
    pushl restart_enter
save_ret:
    pushl %esi                  // restore the caller's address
    ret
