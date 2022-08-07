    .section .text
    .globl _start
_start:
    call kinit
    call kmain
    jmp .
