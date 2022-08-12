#include <sys/const.h>

    .globl _start
    .globl ker_stackbase

    .section .bss
    .lcomm ker_stackbase, KER_STACK_SIZE
ker_stacktop:

    .section .text
_start:
    movl $ker_stacktop, %esp

    call kinit
    call kmain
    jmp .
