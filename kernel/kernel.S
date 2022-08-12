#include <sys/const.h>


    .section .bss
    .lcomm ker_stackbase, KER_STACK_SIZE
ker_stacktop:

    .section .text
    .globl _start
_start:
    movl $ker_stacktop, %esp

    call kinit
    call kmain
    jmp .
