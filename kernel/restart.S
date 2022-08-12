#include "global.h"
#include <sys/proc.h>


    .section .text
    .global restart
    .global save

    .type restart, @function
restart:
    movl curr_proc, %esp
    lldt PROC_OFF_LDT(%esp)
    leal PROC_OFF_SS(%esp), %eax
    movl %eax, (tss + )

    ret


    .type save, @function
save:
    ret
