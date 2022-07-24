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


    # set GDT code descroptor base
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

    # set rin3 descriptor base
    movl $code_ring_3, %eax
    movl $GDT_R3_CODE, %ebx
    call set_dt

    # set global stack base
    movl $global_stack_section, %eax
    movl $GDT_GLOBAL_STACK, %ebx
    call set_dt

    # set rin3 stack base
    movl $r3_stack_section, %eax
    movl $GDT_R3_STACK, %ebx
    call set_dt

    # set TSS descriptor base
    movl $TSS_SECTION, %eax
    movl $GDT_TSS, %ebx
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
    movw $GDT_SELECTOR_STACK, %ax
    movw %ax, %ss
    movl $global_stack_section_len, %esp    # stack top


    // movl $init_loader_msg, %esi
    // call print_string_32

    call $CG_SELECTOR_CODE, $0x0

    movw $GDT_SELECTOR_TSS, %ax
    ltr %ax

    # push type should be consistent with stack alignment
    pushl $GDT_SELECTOR_R3_STACK
    pushl $r3_stack_section_len             # stack top of r3
    pushl $GDT_SELECTOR_R3_CODE
    pushl $0
    retf

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
    .type call_gate_code, @function
call_gate_code:
    movw $GDT_SELECTOR_VGA_MEM, %ax
    movw %ax, %ds
    movl $0, %ebx
    movw $0x0f43, %ax           # print C
    movw %ax, (%ebx)
call_gate_code_ret:
    retf

    .set call_gate_code_len, (. - call_gate_code)


    .code32
    .type code_ring_3, @function
code_ring_3:
    movw $GDT_SELECTOR_VGA_MEM, %ax
    movw %ax, %ds
    movl $0, %ebx
    movw $0x0f33, %ax       # print 3
    movw %ax, (%ebx)
    // ring3 to ring0
    call $CG_SELECTOR_CODE, $0x0


code_ring_3_fin:
    # Since issuing the HLT instruction requires ring 0 access,
    # it can only be run by privileged system software such as
    # the kernel.
    // hlt
    jmp code_ring_3_fin

    .set code_ring_3_len, (. - code_ring_3)

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


    .align 32
GDT_0:
    DEF_GDT_NULL        # NULL, 0x0
GDT_CODE:
    DEF_GDT(
        0x0,
        protect_mod_len,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_E | GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # code, 0x8
GDT_DATA:
    DEF_GDT(
        0x0,
        0xfffff,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # data, 0x10
GDT_LDT:
    # need to reset with real base
    DEF_GDT(
        0x0,
        0x7,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_RW,
        0x0
    )                   # LDT, 0x18
GDT_CG_CODE:
    # need to reset with real base
    DEF_GDT(
        0x0,
        call_gate_code_len,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_E | GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # call gate, 0x20
GDT_TASK:
    DEF_GDT_NULL        # task (not used yet.), 0x28
CG_0:
    DEF_CALL_GATE(
        0x0,
        GDT_SELECTOR_CG,
        0,
        CALL_GATE_32,
        CALL_GATE_PL3,
        CALL_GATE_P
    )                   # call gate, 0x30
GDT_R3_CODE:
    # need to reset with real base
    DEF_GDT(
        0x0,
        code_ring_3_len,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_E | GDT_ACCESS_BYTE_RW |
            GDT_ACCESS_BYTE_DPL3,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # call gate, 0x38
GDT_GLOBAL_STACK:
    DEF_GDT(
        0x0,
        global_stack_section_len,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # global stack, 0x40
GDT_R3_STACK:
    DEF_GDT(
        0x0,
        r3_stack_section_len,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_RW | GDT_ACCESS_BYTE_DPL3,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # r3 stack, 0x48
GDT_VGA_MEM:
    DEF_GDT(
        0x0b8000,
        0xfffff,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_RW | GDT_ACCESS_BYTE_DPL3,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # VGA memory, 0x50
GDT_TSS:
    # need to reset with real base
    DEF_GDT(
        0x0,
        tss_len,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_E |
            GDT_ACCESS_BYTE_A,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   # TSS, 0x58


GDTR0:
    .short (GDTR0 - GDT_0) - 1
    .int GDT_0


LDT0:
    DEF_LDT(
        0x0,
        0xfffff,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_E,
        GDT_FLAGS_DB
    )               # ldt for test, 0x0


#############################################################
# Global stack in protect mode, the size is 512 bytes.      #
#############################################################
    .align 32
    .code32
global_stack_section:
    .fill 512, 1, 0
    .set global_stack_section_len, (. - global_stack_section)


#############################################################
# Global stack in protect mode / r3, the size is 512 bytes. #
#############################################################
    .align 32
    .code32
r3_stack_section:
    .fill 512, 1, 0
    .set r3_stack_section_len, (. - r3_stack_section)


    .code32
    .align 32
TSS_SECTION:
    DEF_TSS(global_stack_section_len, GDT_SELECTOR_STACK)
    .set tss_len, (. - TSS_SECTION)

    // .align 16
    // .code16
init_loader_msg:
    .asciz "Init loader ...\r\n"
init_GDT_msg:
    .asciz "Init GDT ...\r\n"


    .include "include/boothdr.S"

