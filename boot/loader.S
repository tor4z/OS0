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


    # set call gate descroptor base
    movl $protect_mod, %eax
    movl $GDT_CODE, %ebx
    call set_dt

    # set call gate descroptor base
    movl $call_gate_code, %eax
    movl $GDT_CG_CODE, %ebx
    call set_dt

    # set LDT descriptor base
    movl $LDT0, %eax
    movl $GDT_LDT, %ebx
    call set_dt

    lgdt GDTR0
    movl %cr0, %eax
    or $0x1, %eax
    movl %eax, %cr0

    jmp $GDT_SELECTOR_CODE, $0x0



    .code32
protect_mod:
    movw $GDT_SELECTOR_DATA, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    movl $init_loader_msg, %esi
    call print_string_32

    call $CG_SELECTOR_CODE, $0x0

    // jump to ldt
    movw $GDT_SELECTOR_LDT, %ax
    lldt %ax
    jmp $LDT_SELECTOR_CODE, $ldt_protect_mode
protect_mod_fin:
    hlt
    jmp protect_mod_fin

    .set protect_mod_len, (. - protect_mod)



    .code32
ldt_protect_mode:
    movw $0x0f4c, %ax           # print L
    movl $VIDEO_MEM, %ebx
    movw %ax, (%ebx)
ldt_protect_mode_fin:
    hlt
    jmp ldt_protect_mode_fin


    .code32
call_gate_code:
    movw $0x0f43, %ax           # print C
    movl $VIDEO_MEM, %ebx
    movw %ax, (%ebx)
call_gate_code_ret:
    retf

    .set call_gate_code_len, (. - call_gate_code)

###############################################################
# @function: set the base of gdt/ldt                          #
# @parameter:                                                 #
#   eax: base of the segment, required                        #
#   ebx: the address of the descriptor, required              #
# @return:                                                    #
#   Nothing                                                   #
###############################################################
    .code16
    .type set_dt, @function
set_dt:
    addl $2, %ebx
    movw %ax, (%ebx)
    shrl $16, %eax
    addl $2, %ebx
    movb %al, (%ebx)
    addl $3, %ebx
    movb %ah, (%ebx)
set_dt_ret:
    ret


###############################################################
# @function: print a string in 32-bit protection mode         #
# @parameter:                                                 #
#   si: the string source                                     #
# @return:                                                    #
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

    movl (SCRN_W_ADDR), %ecx
    movl (SCRN_Y_ADDR), %edx
    movl %edx, %eax
    mull %ecx
    movl %eax, %edx
    movl (SCRN_X_ADDR), %ecx
    addl %ecx, %edx
    shll %edx
    addl %edx, %ebx
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
    shll %ecx
    subl %ecx, %ebx
    xorl %ecx, %ecx
    jmp print_string_32_loop
print_string_32_n:                  # \n
    inc %edx
    cmpl (SCRN_H_ADDR), %edx
    je print_string_32_scroll
    addl (SCRN_W_ADDR * 2), %ebx
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
    pushl %eax
    pushl %ebx
    pushl %ecx

    decl (SCRN_Y_ADDR)
    movl $1920, %ecx
    shll %ecx
    movl $VIDEO_MEM, %esi
    movl (SCRN_W_ADDR), %eax
    shll %eax
    addl %eax, %esi
    movl $VIDEO_MEM, %ebx
scroll_line_loop:
    lodsb
    movb %al, (%ebx)
    incl  %ebx
    loop scroll_line_loop
scroll_line_space:
    movl (SCRN_W_ADDR), %ecx
scroll_line_space_loop:
    movw $0x0f20, (%ebx)
    addl $2, %ebx
    loop scroll_line_space_loop
scroll_line_ret:
    popl %ecx
    popl %ebx
    popl %eax
    popl %esi

    ret



    .code16
wait_kb_out:
    inb $0x64, %al
    test $0x02, %al
    jnz wait_kb_out
    ret


    .align 16
GDT_0:
    DEF_GDT_NULL        # NULL
GDT_CODE:
    DEF_GDT(
        0x0,
        protect_mod_len - 1,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_E | GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # code
GDT_DATA:
    DEF_GDT(
        0x0,
        0xfffff,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # data
GDT_LDT:
    # need to reset with real base
    DEF_GDT(
        0x0,
        0x7,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_RW,
        0x0
    )                   # LDT
GDT_CG_CODE:
    # need to reset with real base
    DEF_GDT(
        0x0,
        call_gate_code_len - 1,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_E | GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # call gate
GDT_TASK:
    DEF_GDT_NULL        # task (not used yet.)
CG_0:
    DEF_CALL_GATE(
        0x0,
        GDT_SELECTOR_CG,
        0,
        CALL_GATE_32,
        CALL_GATE_PL0,
        CALL_GATE_P
    )


GDTR0:
    .short (GDTR0 - GDT_0) - 1
    .int GDT_0


    .align 32
LDT0:
    DEF_LDT(
        0x0,
        0xfffff,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_E,
        GDT_FLAGS_DB
    )


init_loader_msg:
    .asciz "Init loader ...\r\n"
init_GDT_msg:
    .asciz "Init GDT ...\r\n"


    .include "include/boothdr.S"

