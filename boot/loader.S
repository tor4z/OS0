#include "loader.h"

    // .set VIDEO_MEM, 0xb8000
    // .set SCRN_W_ADDR, 0x9600    # To store max screen cloume (4 bytes)
    // .set SCRN_H_ADDR, 0x9604    # To store max screen row (4 bytes)
    // .set SCRN_X_ADDR, 0x9608    # To store current screen cloume (4 bytes)
    // .set SCRN_Y_ADDR, 0x960c    # To store current screen row (4 bytes)

#define VIDEO_MEM 0xb8000
#define SCRN_W_ADDR 0xa000    // To store max screen cloume (4 bytes)
#define SCRN_H_ADDR 0xa004    // To store max screen row (4 bytes)
#define SCRN_X_ADDR 0xa008    // To store current screen cloume (4 bytes)
#define SCRN_Y_ADDR 0xa00c    // To store current screen row (4 bytes)


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

    movb $80, %ds:SCRN_W_ADDR
    movb $25, %ds:SCRN_H_ADDR
    movb $0, %ds:SCRN_X_ADDR
    movb $0, %ds:SCRN_Y_ADDR

    // movb $0x02, %ah
    // int $0x16
    // movb %al, (LEDS)

    leaw %ds:memory_info_buff, %di
    call fetch_mem_info

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


###############################################################
# @function: fatch system memory map                          #
# @parameter:                                                 #
#   di: The buffer address to store memory info               #
# @return:                                                    #
#   Nothing                                                   #
###############################################################
    .code16
    .type fetch_mem_info, @function
fetch_mem_info:
    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx

    xorl %ebx, %ebx
fetch_mem_info_loop:
    movl $20, %ecx
    movl $0x0000e820, %eax
    movl $0x534d4150, %edx
    int $0x15
    jc fetch_mem_info_fail
    incl %ds:memory_info_item_cnt
    addw $20, %di
    cmpl $0, %ebx
    jne fetch_mem_info_loop
    jmp fetch_mem_info_success
fetch_mem_info_fail:
    movl $MEM_INFO_FAIL, %eax
    movl %eax, %ds:memory_info_status
fetch_mem_info_success:
    movl $MEM_INFO_SUCCESS, %eax
    movl %eax, %ds:memory_info_status
fetch_mem_info_ret:
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax
    ret


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
    pushl %ecx

    movl $2, %ecx
    movw %ax, (%ebx, %ecx, 1)
    shrl $16, %eax
    addl $2, %ecx
    movb %al, (%ebx, %ecx, 1)
    addl $3, %ecx
    movb %ah, (%ebx, %ecx, 1)
set_dt_ret:
    popl %ecx
    ret


###############################################################
# @function: wait keyboard out                                #
# @parameter:                                                 #
#   Nothing                                                   #
# @return:                                                    #
#   Nothing                                                   #
###############################################################
    .code16
    .type wait_kb_out, @function
wait_kb_out:
wait_kb_out_loop:
    pushw %ax

    inb $0x64, %al
    test $0x02, %al
    jnz wait_kb_out_loop
wait_kb_out_ret:
    popw %ax
    ret


    .code32
protect_mod:
    movw $GDT_SELECTOR_DATA, %ax
    movw %ax, %ds
    movw %ax, %gs
    movw %ax, %fs

    movw $GDT_SELECTOR_VGA_MEM, %ax
    movw %ax, %es

    movw $GDT_SELECTOR_STACK, %ax
    movw %ax, %ss
    movl $global_stack_section_len, %esp    # stack top

    leaw %ds:memory_info_buff, %di
    call show_memory_map
    call setup_page

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


###############################################################
# @function: setup page                                       #
# @parameter:                                                 #
#   Nothing                                                   #
# @return:                                                    #
#   Nothing                                                   #
###############################################################
    .type setup_page, @function
setup_page:
    pushl %eax
    pushl %ecx
    pushl %edi
    pushw %es

    movw $GDT_SELECTOR_PG_DIR, %ax
    movw %ax, %es

    xorl %edi, %edi
    xorl %eax, %eax
    movl %es:(PAGE_TBL_BASE | PG_P | PG_RW_RW | PG_US_U), %eax
    movl $1024, %ecx
.pe:
    stosl
    addl $4096, %eax
    loop .pe

    movw $GDT_SELECTOR_PG_TBL, %ax
    movw %ax, %es

    xorl %edi, %edi
    xorl %eax, %eax
    movl %es:(PG_P | PG_RW_RW | PG_US_U), %eax
    movl $1048576, %ecx                         # 1024 * 1024
.pt:
    stosl
    addl $4096, %eax
    loop .pt

    movl $PAGE_DIR_BASE, %eax
    movl %eax, %cr3
    movl %cr0, %eax
    orl $0x80000000, %eax
    movl %eax, %cr0
setup_page_ret:
    popw %es
    popl %edi
    popl %ecx
    popl %eax
    ret


###############################################################
# @function: Show the system memory map                       #
# @ parameter:                                                #
#   di: The memory info buffer                                #
# @return:                                                    #
#   Nothing                                                   #
###############################################################
    .code32
    .type show_memory_map, @function
show_memory_map:
    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %esi

    movl %ds:memory_info_item_cnt, %ecx
    movl %ecx, %edx
    call print_int_32
    call print_rn_32

    movl $memory_info_buff, %ebx
    xorl %esi, %esi

show_memory_map_loop:
    movl %ds:0(%ebx, %esi, 4), %edx
    call print_int_32
    incl %esi

    call print_space_32

    movl %ds:0(%ebx, %esi, 4), %edx
    call print_int_32
    incl %esi

    call print_space_32

    movl %ds:0(%ebx, %esi, 4), %edx
    call print_int_32
    incl %esi

    call print_space_32

    movl %ds:0(%ebx, %esi, 4), %edx
    call print_int_32
    incl %esi

    call print_space_32

    movl %ds:0(%ebx, %esi, 4), %edx
    call print_int_32
    incl %esi

    call print_rn_32
    loop show_memory_map_loop

show_memory_map_ret:
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax
    ret


###############################################################
# @function: print integer in 32-bit protect mode             #
# @ parameter:                                                #
#   edx: The integer                                          #
# @return:                                                    #
#   Nothing                                                   #
###############################################################
    .code32
    .type print_int_32, @function
print_int_32:
    pushl %eax
    pushl %ecx
    pushl %ebx
    pushl %edi

    pushl %edx

    xorl %ebx, %ebx

    movl %ds:SCRN_W_ADDR, %ecx
    movl %ds:SCRN_Y_ADDR, %edi
    movl %edi, %eax
    mull %ecx
    movl %eax, %edi
    movl %ds:SCRN_X_ADDR, %ecx
    addl %ecx, %edi
    addl %edi, %ebx
    movl %ds:SCRN_X_ADDR, %ecx
    movl %ds:SCRN_Y_ADDR, %edi

    popl %edx
    movb $0x0f, %ah                 // set color    
    movb $0x30, %al                  // print 0
    movw %ax, %es:0(, %ebx, 2)

    incl %ebx
    movb $0x78, %al                  // print x
    movw %ax, %es:0(, %ebx, 2)

    incl %ebx
    movl %edx, %eax
    shrl $24, %eax
    call print_al                // the first byte

    movl %edx, %eax
    shrl $16, %eax
    call print_al                // the second byte

    movl %edx, %eax
    shrl $8, %eax
    call print_al                // the third byte

    movl %edx, %eax
    call print_al                // the fourth byte

    addl $10, %ecx
    cmpl %ds:SCRN_W_ADDR, %ecx
    je print_int_32_wrap
    jmp print_int_32_ret
print_int_32_wrap:
    incl %edx
    xorl %ecx, %ecx
    cmpl %ds:SCRN_H_ADDR, %edi
    je print_int_32_scroll
    jmp print_int_32_ret
print_int_32_scroll:
    decl %edi
    call scroll_line
print_int_32_ret:
    movl %ecx, %ds:SCRN_X_ADDR
    movl %edi, %ds:SCRN_Y_ADDR

    popl %edi
    popl %ebx
    popl %ecx
    popl %eax
    ret


###############################################################
# @function: print the number in al register                  #
# @parameter:                                                 #
#   al: The number to print                                   #
#   ebx: offset in VGA memory                                 #
# @return:                                                    #
#   ebx: current VAG memory offset                            #
###############################################################
    .code32
    .type print_al, @function
print_al:
    pushl %edx
    pushl %ecx

    movl $2, %ecx
    xorl %edx, %edx
    movb %al, %dl
    movb %al, %dh
    movb $0x0f, %ah
print_al_loop:
    shrw $4, %dx
    movb %dl, %al
    andb $0x0f, %al
    cmpb $9, %al                 # compare with 9
    jbe print_al_be9
    jmp print_al_a9
print_al_be9:
    addb $0x30, %al
    jmp print_al_to_print
print_al_a9:
    addb $0x61, %al
    subb $0x0a, %al
print_al_to_print:
    movw %ax, %es:0(, %ebx, 2)
    incl %ebx
    loop print_al_loop
print_al_ret:
    popl %ecx
    popl %edx
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
    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx

    xorl %ebx, %ebx

    movl %ds:SCRN_W_ADDR, %ecx
    movl %ds:SCRN_Y_ADDR, %edx
    movl %edx, %eax
    mull %ecx
    movl %eax, %edx
    movl %ds:SCRN_X_ADDR, %ecx
    addl %ecx, %edx
    addl %edx, %ebx
    movl %ds:SCRN_X_ADDR, %ecx
    movl %ds:SCRN_Y_ADDR, %edx
print_string_32_begin:
    movb $0x0f, %ah
    lodsb
    cmpb $0x0, %al
    je print_string_32_ret
    cmpb $0x0d, %al
    je print_string_32_r
    cmpb $0x0a, %al
    je print_string_32_n
    jmp print_string_32_printtable
print_string_32_r:                  # \r
    subl %ecx, %ebx
    xorl %ecx, %ecx
    jmp print_string_32_begin
print_string_32_n:                  # \n
    inc %edx
    cmpl %ds:SCRN_H_ADDR, %edx
    je print_string_32_scroll
    addl %ds:SCRN_W_ADDR, %ebx
    jmp print_string_32_begin
print_string_32_printtable:
    movw %ax, %es:0(, %ebx, 2)
    inc %ecx
    cmpl %ds:SCRN_W_ADDR, %ecx
    je print_string_32_wrap
    incl %ebx
    jmp print_string_32_begin
print_string_32_wrap:
    inc %edx
    xorl %ecx, %ecx
    cmpl %ds:SCRN_H_ADDR, %edx
    je print_string_32_scroll
    incl %ebx
    jmp print_string_32_begin
print_string_32_scroll:
    dec %edx
    call scroll_line
print_string_32_ret:
    movl %ecx, %ds:SCRN_X_ADDR
    movl %edx, %ds:SCRN_Y_ADDR

    popl %edx
    popl %ecx
    popl %ebx
    popl %eax
    ret


###############################################################
# @function: print "\r\n"                                     #
# @parameter:                                                 #
#   Nothing                                                   #
# @return:                                                    #
#   Nothing                                                   #
###############################################################
    .code32
    .type print_rn_32, @function
print_rn_32:
    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx

    xorl %ebx, %ebx

    movl %ds:SCRN_W_ADDR, %ecx
    movl %ds:SCRN_Y_ADDR, %edx
    movl %edx, %eax
    mull %ecx
    movl %eax, %edx
    movl %ds:SCRN_X_ADDR, %ecx
    addl %ecx, %edx
    addl %edx, %ebx
    movl %ds:SCRN_X_ADDR, %ecx
    movl %ds:SCRN_Y_ADDR, %edx
print_rn_32_begin:
    xorl %ecx, %ecx
    incl %edx
    cmpl %ds:SCRN_H_ADDR, %edx
    je print_rn_32_scroll
    jmp print_rn_32_ret
print_rn_32_scroll:
    decl %edx
    call scroll_line
print_rn_32_ret:
    movl %ecx, %ds:SCRN_X_ADDR
    movl %edx, %ds:SCRN_Y_ADDR

    popl %edx
    popl %ecx
    popl %ebx
    popl %eax

    ret


###############################################################
# @function: print a white space                              #
# @parameter:                                                 #
#   Nothing                                                   #
# @return:                                                    #
#   Nothing                                                   #
###############################################################
    .code32
    .type print_space_32, @function
print_space_32:
    pushl %eax
    pushl %ebx
    pushl %ecx
    pushl %edx
    pushl %edi

    xorl %edi, %edi

    movl %ds:SCRN_W_ADDR, %ecx
    movl %ds:SCRN_Y_ADDR, %edx
    movl %edx, %eax
    mull %ecx
    movl %eax, %edx
    movl %ds:SCRN_X_ADDR, %ecx
    addl %ecx, %edx
    addl %edx, %edi
    movl %ds:SCRN_X_ADDR, %ecx
    movl %ds:SCRN_Y_ADDR, %edx
print_space_32_begin:
    movw $0x0f20, %ax
    movw %ax, %es:0(, %edi, 2)
    incl %ecx
    cmpl %ds:SCRN_W_ADDR, %ecx
    je print_space_32_wrap
    jmp print_space_32_ret
print_space_32_wrap:
    xorl %ecx, %ecx
    incl %edx
    cmpl %ds:SCRN_H_ADDR, %edx
    je print_space_32_scroll
    jmp print_space_32_ret
print_space_32_scroll:
    decl %edx
    call scroll_line
print_space_32_ret:
    movl %ecx, %ds:SCRN_X_ADDR
    movl %edx, %ds:SCRN_Y_ADDR

    popl %edi
    popl %edx
    popl %ecx
    popl %ebx
    popl %eax

    ret

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

    decl %ds:SCRN_Y_ADDR
    movl $1920, %ecx
    shll %ecx
    movl $VIDEO_MEM, %esi
    movl %ds:SCRN_W_ADDR, %eax
    shll %eax
    addl %eax, %esi                 // point to the begin of second line
    movl $VIDEO_MEM, %ebx
scroll_line_loop:
    lodsb
    movb %al, %ds:0(%ebx, 1)
    incl  %ebx
    loop scroll_line_loop
scroll_line_space:
    movl %ds:SCRN_W_ADDR, %ecx
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
GDT_PG_DIR:
    # need to reset with real base
    DEF_GDT(
        PAGE_DIR_BASE,
        4096,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G
    )                   # page dir, 0x60
GDT_PG_TBL:
    # need to reset with real base
    DEF_GDT(
        PAGE_TBL_BASE,
        1024,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G
    )                   # page table, 0x68

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


memory_info_buff:
    .fill 256, 1, 0
    .set memory_info_buff_len, (. - memory_info_buff)
memory_info_status:
    // 0: not init yet
    // 1: succrss
    // 2: fail
    .int 0
memory_info_item_cnt:
    .int 0


    // .align 16
    // .code16
init_loader_msg:
    .asciz "Init loader ...\r\n"
init_GDT_msg:
    .asciz "Init GDT ...\r\n"


    .include "include/boothdr.S"

