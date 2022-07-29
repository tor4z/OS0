    .section .text
    .globl _start
_start:
    movb $0x0f, %ah
    movb $0x4b, %al
    movw %ax, %es:0
    jmp .
