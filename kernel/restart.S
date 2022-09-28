#include <sys/const.h>

    .extern curr_proc
    .extern ker_stacktop
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
    leal PROC_OFF_STACKTOP(%esp), %eax
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

    movl %esp, %esi
    movw %ss, %dx
    movw %dx, %ds
    movw %dx, %es
    movw %dx, %fs
    movw %dx, %gs

    incl kreenter
    cmpl $0, kreenter
    jne in_kernel
not_kernel:
    movl ker_stacktop, %esp   /* kernel stack top */
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
