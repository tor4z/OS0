#include "loader.h"

    // .set VIDEO_MEM, 0xb8000
    // .set SCRN_W_ADDR, 0x9600    # To store max screen cloume (4 bytes)
    // .set SCRN_H_ADDR, 0x9604    # To store max screen row (4 bytes)
    // .set SCRN_X_ADDR, 0x9608    # To store current screen cloume (4 bytes)
    // .set SCRN_Y_ADDR, 0x960c    # To store current screen row (4 bytes)

#define VIDEO_MEM 0xb8000
#define SCRN_W_ADDR 0x9600    // To store max screen cloume (4 bytes)
#define SCRN_H_ADDR 0x9604    // To store max screen row (4 bytes)
#define SCRN_X_ADDR 0x9608    // To store current screen cloume (4 bytes)
#define SCRN_Y_ADDR 0x960c    // To store current screen row (4 bytes)


    .code16
    .globl start
start:
    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %gs
    movw %ax, %ss

    movb $0x00, %ah
    movb $0x03, %al
    int $0x10

    movb $80, (SCRN_W_ADDR)
    movb $25, (SCRN_H_ADDR)
    movb $0, (SCRN_X_ADDR)
    movb $0, (SCRN_Y_ADDR)

    // movb $0x02, %ah
    // int $0x16
    // movb %al, (LEDS)

    movb $0xff, %al
    outb %al, $0x21
    outb %al, $0xa1
    cli

    call wait_kb_out
    movb $0xd1, %al
    outb %al, $0x64
    call wait_kb_out
    movb $0xdf, %al
    outb %al, $0x60
    call wait_kb_out

    lgdt GDTR0
    movl %cr0, %eax
    or $0x1, %eax
    movl %eax, %cr0

    jmp $0x8, $protect_mod


    .code32
protect_mod:
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    movl $VIDEO_MEM, %ebx
    movw $0x0f21, %ax
    movw %ax, (%ebx)
    movw $0x0f21, %ax
    addl $2, %ebx
    movw %ax, (%ebx)
    movw $0x0f21, %ax
    addl $2, %ebx
    movw %ax, (%ebx)



    movl $init_loader_msg, %esi
    call print_string_32
fin:
    hlt
    jmp fin


###############################################################
# @function: print a string in 32-bit protection mode         #
# @parameter:                                                 #
#   si: the string source                                     #
# @return                                                     #
#   Nothing                                                   #
###############################################################
    .code32
    .type print_string_32, @function
print_string_32:
    pushw %ax
    pushl %ebx
    pushl %ecx
    pushl %edx

    movl $VIDEO_MEM, %ebx

    movb $0x0f, %ah
    movb $0x21, %al
    movw %ax, (%ebx)

    movl (SCRN_X_ADDR), %ecx
    movl (SCRN_Y_ADDR), %edx
print_string_32_loop:
    movb $0x0f, %ah
    lodsb
    cmpb $0x0, %al
    je print_string_32_ret
    cmpb $0x0d, %al
    je print_string_32_r
    cmpb $0x0a, %al
    je print_string_32_n
    jmp print_string_32_printable
print_string_32_r:                  # \r
    subl %ecx, %ebx
    xorl %ecx, %ecx
    jmp print_string_32_loop
print_string_32_n:                  # \n
    inc %edx
    cmpl (SCRN_H_ADDR), %edx
    je print_string_32_scroll
    addl (SCRN_W_ADDR), %ebx
    jmp print_string_32_loop
print_string_32_printable:
    movw %ax, (%ebx)
    inc %ecx
    cmpl (SCRN_W_ADDR), %ecx
    je print_string_32_line_wrap
    addl $2, %ebx
    jmp print_string_32_loop
print_string_32_line_wrap:
    inc %edx
    xorl %ecx, %ecx
    cmpl (SCRN_H_ADDR), %edx
    je print_string_32_scroll
    addl $2, %ebx
    jmp print_string_32_loop
print_string_32_scroll:
    dec %edx
    call scroll_line
print_string_32_ret:
    movl %ecx, (SCRN_X_ADDR)
    movl %edx, (SCRN_Y_ADDR)

    popl %edx
    popl %ecx
    popl %ebx
    popw %ax
    ret


###############################################################
# @function: scroll one line                                  #
# @parameter:                                                 #
#   Nothing                                                   #
# @return                                                     #
#   Nothing                                                   #
###############################################################
    .code32
    .type scroll_line, @function
scroll_line:
    pushl %esi
    pushl %ebx
    pushl %ecx

    decl (SCRN_Y_ADDR)
    movl $1920, %ecx
    movl $VIDEO_MEM, %esi
    addl $80, %esi
    movl $VIDEO_MEM, %ebx
scroll_line_loop:
    lodsb
    movb %al, (%ebx)
    inc %ebx
    loop scroll_line_loop
scroll_line_ret:
    popl %ecx
    popl %ebx
    popl %esi

    ret



    .code16
wait_kb_out:
    inb $0x64, %al
    test $0x02, %al
    jnz wait_kb_out
    ret


    .align 16
GDT0:
    DEF_GDT_NULL        # NULL
    DEF_GDT(
        0x0,
        0xfffff,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_E | GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # code
    DEF_GDT(
        0x0,
        0xfffff,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # data
    DEF_GDT_NULL        # task (not used yet.)


GDTR0:
    .short 4 * 8 - 1
    .int GDT0



init_loader_msg:
    .asciz "Init loader ...\r\n"
init_GDT_msg:
    .asciz "Init GDT ...\r\n"


    .include "include/boothdr.S"

