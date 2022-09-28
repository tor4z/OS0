#include <sys/const.h>

    .extern ker_stacktop 
    .globl _start
    .globl ker_stackbase

    .section .bss
    .lcomm ker_stackbase, KER_STACK_SIZE


    .section .text
_start:
    movl $ker_stackbase, %eax
    addl $KER_STACK_SIZE, %eax
    movl %eax, ker_stacktop
    movl ker_stacktop, %esp

    call kinit
    call kmain
    /* We will never reach here. */
    jmp .
