#include <sys/const.h>

    .extern curr_proc
    .extern ker_stackbase
    .extern inc_kreenter
    .extern dec_kreenter
    .extern master_int

    .section .text
    .global restart
    .global save

    .type restart, @function
restart:
    movl curr_proc, %esp
    lldt PROC_OFF_LDT_SELECTOR(%esp)
    // leal PROC_OFF_SS(%esp), %eax
    movl $tss, %esi
    movl %ss, %eax
    movl %eax, TSS_OFF_SS0(%esi)
    movl %esp, TSS_OFF_ESP0(%esi)
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

    movl %esp, %esi
not_kernel:
    movl $ker_stackbase, %esp   // kernel stack top
    pushl $restart
    jmp save_ret
in_kernel:
    pushl $restart_enter
save_ret:
    jmp PROC_OFF_RET(%esi)


    .global irq0_handler
    .type irq0_handler, @function
irq0_handler:
    call save
    pushl $0
    call master_int
    addl $4, %esp
    ret
