#include "mmu.h"

/*************************************************
 *              16-bit code                      *
 *************************************************/

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

    movl $80, %ds:SCRN_W_ADDR
    movl $25, %ds:SCRN_H_ADDR
    movl $0, %ds:SCRN_X_ADDR
    movl $0, %ds:SCRN_Y_ADDR

read_kernel:
    call reset_sector
    leaw kernel_filename, %bx
    call find_file
    /* the start logical cluster is stored in ax register */
    movw $KERNEL_TMP_ADDR, %bx
    call read_file
    movw %cx, %ds:KERNEL_SIZE

    /*
    movb $0x02, %ah
    int $0x16
    movb %al, (LEDS)
    */

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

    // /* set GDT code descroptor base */
    // movl $protect_mod, %eax
    // movl $GDT_CODE, %ebx
    // call set_dt

    // /* set call gate descroptor base */
    // movl $call_gate_code, %eax
    // movl $GDT_CG_CODE, %ebx
    // call set_dt

    // /* set LDT descriptor base */
    // movl $LDT0, %eax
    // movl $GDT_LDT, %ebx
    // call set_dt

    // /* set rin3 descriptor base */
    // movl $code_ring_3, %eax
    // movl $GDT_R3_CODE, %ebx
    // call set_dt

    // /* set global stack base */
    // movl $global_stack_section, %eax
    // movl $GDT_GLOBAL_STACK, %ebx
    // call set_dt

    // /* set rin3 stack base */
    // movl $r3_stack_section, %eax
    // movl $GDT_R3_STACK, %ebx
    // call set_dt

    /* set TSS descriptor base */
    // movl $TSS_SECTION, %eax
    // movl $GDT_TSS, %ebx
    // call set_dt

    lgdt GDTR0
    movl %cr0, %eax
    or $0x1, %eax
    movl %eax, %cr0

    jmp $GDT_SELECTOR_CODE, $protect_mod



/**************************************************************
 * @function: set the base of gdt/ldt                         *
 * @parameter:                                                *
 *  eax: base of the segment, required                        *
 *  ebx: the address of the descriptor, required              *
 * @return:                                                   *
 *  Nothing                                                   *
 **************************************************************/
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


/*************************************************
 * @function: wait keyboard out                  *
 * @parameter:                                   *
 *  Nothing                                      *
 * @return:                                      *
 *  Nothing                                      *
 *************************************************/
    .code16
    .type wait_kb_out, @function
wait_kb_out:
    inb $0x64, %al
    test $0x02, %al
    jnz wait_kb_out
    ret

    .include "include/boothdr.S"

/*************************************************
 *              32-bit code                      *
 *************************************************/

    .code32
protect_mod:
    movw $GDT_SELECTOR_DATA, %ax
    movw %ax, %ds
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %es
    movw %ax, %ss
    movl $global_stack_section_len, %esp    /* stack top */

    /*
    movl $init_loader_msg, %esi
    call print_string_32
    */

    // call $CG_SELECTOR_CODE, $0x0

    movw $GDT_SELECTOR_TSS, %ax
    ltr %ax

    pushl $KERNEL_TMP_ADDR
    call static_elf_loader
    addl $4, %esp

    jmp KERNEL_ENTRY

    // /* push type should be consistent with stack alignment */
    // pushl $GDT_SELECTOR_R3_STACK
    // pushl $r3_stack_section_len             /* stack top of r3 */
    // pushl $GDT_SELECTOR_R3_CODE
    // pushl $0
    // retf

    // // jump to ldt
    // movw $GDT_SELECTOR_LDT, %ax
    // lldt %ax
    // jmp $LDT_SELECTOR_CODE, $ldt_protect_mode
protect_mod_fin:
    hlt
    jmp protect_mod_fin


/*******************************************************
 * @function: static ELF loader                        *
 * @parameter:                                         *
 *   8(ebp): the elf address                           *
 * @return:                                            *
 *   Nothing                                           *
 *******************************************************/
    .code32
    .type static_elf_loader, @function
static_elf_loader:
    pushl %ebp
    movl %esp, %ebp
    subl $4, %esp
    /* local variable
     * -4(ebp): curr_ph_addr
     */
    pushl %ecx
    pushl %eax
    pushl %esi
    pushl %edi

    movl 8(%ebp), %eax
    movzx ELF_PHNUM_OFFSET(%eax), %ecx        /* read the number program header */
    movl 8(%ebp), %eax
    addl $ELF_PH_OFFSET, %eax               /* skip ELF file header */
    movl %eax, -4(%ebp)
read_elf_ph:
    movl -4(%ebp), %esi
    movl ELF_PH_TYPE_OFFSET(%esi), %eax
    cmpl $ELF_PT_LOAD, %eax                 /* jump to next item if it is */
    jnz read_next_elf_ph                    /* not a loadable segment */
    movl ELF_PH_FILESZ_OFFSET(%esi), %eax
    pushl %eax
    movl ELF_PH_VADDR_OFFSET(%esi), %eax
    pushl %eax
    movl ELF_PH_OFFSET_OFFSET(%esi), %eax
    addl 8(%ebp), %eax
    pushl %eax
    call memcpy
    addl $12, %esp
read_next_elf_ph:
    addl $ELF_PH_SIZE, -4(%ebp)
    loop read_elf_ph
init_bss:
    movl 8(%ebp), %eax
    movl ELF_SHOFF_OFFSET(%eax), %esi
    addl 8(%ebp), %esi
    movzx ELF_SHNUM_OFFSET(%eax), %ecx
read_elf_sh:
    movl ELF_SH_TYPE_OFFSET(%esi), %eax
    cmpl $ELF_SHT_NOBITS, %eax
    jnz read_next_elf_sh
set_bss_area_zero:
    movl ELF_SH_SIZE_OFFSET(%esi), %ecx
    movb $0x0, %al
    movl ELF_SH_ADDR_OFFSET(%esi), %edi
    cld
    rep stosb
    jmp static_elf_loader_ret
read_next_elf_sh:
    addl $ELF_SH_SIZE, %esi
    loop read_elf_sh
static_elf_loader_ret:
    addl $4, %esp
    popl %edi
    popl %esi
    popl %eax
    popl %ecx
    movl %ebp, %esp
    popl %ebp
    ret

/*******************************************************
 * @function: memory copy                              *
 * @parameter:                                         *
 *   8(ebp): the source address                        *
 *   12(ebp): the target address                       *
 *   16(ebp): number of byte to copy                   *
 * @return:                                            *
 *   Nothing                                           *
 *******************************************************/
    .code32
    .type memcpy, @function
memcpy:
    pushl %ebp
    movl %esp, %ebp
    pushl %eax
    pushl %ebx
    pushl %esi
    pushl %edi
    pushl %ecx

    movl 8(%ebp), %esi
    movl 12(%ebp), %ebx
    movl 16(%ebp), %ecx
    shrl $2, %ecx
    xorl %edi, %edi
    cld
memcpy_loop:
    lodsl
    movl %eax, (%ebx, %edi, 4)
    incl %edi
    loop memcpy_loop
memcpy_remain:
    movl 16(%ebp), %ecx
    orl $3, %ecx
    leal (%ebx, %edi, 4), %eax
    movl %eax, %ebx
    xorl %edi, %edi
memcpy_remain_loop:
    lodsb
    movb %al, (%ebx, %edi, 1)
    incl %edi
    loop memcpy_remain_loop
memcpy_ret:
    popl %ecx
    popl %esi
    popl %esi
    popl %ebx
    popl %eax
    movl %ebp, %esp
    popl %ebp
    ret

/*******************************************************
 * @function: print a string in 32-bit protection mode *
 * @parameter:                                         *
 *   si: the string source                             *
 * @return:                                            *
 *   Nothing                                           *
 *******************************************************/
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
print_string_32_r:                  /* \r */
    shll %ecx
    subl %ecx, %ebx
    xorl %ecx, %ecx
    jmp print_string_32_loop
print_string_32_n:                  /* \n */
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


/**************************************************
 * @function: scroll one line                     *
 * @parameter:                                    *
 *   Nothing                                      *
 * @return                                        *
 *   Nothing                                      *
 **************************************************/
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


    .code32
ldt_protect_mode:
    movw $0x0f4c, %ax           /* print L */
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
    movw $0x0f43, %ax           /* print C */
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
    movw $0x0f33, %ax       /* print 3 */
    movw %ax, (%ebx)
    /* ring3 to ring0 */
    call $CG_SELECTOR_CODE, $0x0
code_ring_3_fin:
    /* Since issuing the HLT instruction requires ring 0 access,
     * it can only be run by privileged system software such as
     * the kernel.
     */
    /* hlt */
    jmp code_ring_3_fin

    .set code_ring_3_len, (. - code_ring_3)


    .align 32
GDT_0:
    DEF_GDT_NULL        /* NULL, 0x0 */
GDT_CODE:
    DEF_GDT(
        0x0,
        0xfffff,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_E | GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   /* code, 0x8 */
GDT_DATA:
    DEF_GDT(
        0x0,
        0xfffff,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   /* data, 0x10 */
GDT_LDT:
    /* need to reset with real base */
    DEF_GDT(
        0x0,
        0x7,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_RW,
        0x0
    )                   /* LDT, 0x18 */
GDT_CG_CODE:
    /* need to reset with real base */
    DEF_GDT(
        0x0,
        call_gate_code_len,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_E | GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   /* call gate, 0x20 */
GDT_TASK:
    DEF_GDT_NULL        /* task (not used yet.), 0x28 */
CG_0:
    DEF_CALL_GATE(
        0x0,
        GDT_SELECTOR_CG,
        0,
        CALL_GATE_32,
        CALL_GATE_PL3,
        CALL_GATE_P
    )                   /* call gate, 0x30 */
GDT_R3_CODE:
    /* need to reset with real base */
    DEF_GDT(
        0x0,
        code_ring_3_len,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_E | GDT_ACCESS_BYTE_RW |
            GDT_ACCESS_BYTE_DPL3,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   /* call gate, 0x38 */
GDT_GLOBAL_STACK:
    DEF_GDT(
        0x0,
        global_stack_section_len,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_RW,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   /* global stack, 0x40 */
GDT_R3_STACK:
    DEF_GDT(
        0x0,
        r3_stack_section_len,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_RW | GDT_ACCESS_BYTE_DPL3,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   /* r3 stack, 0x48 */
GDT_VGA_MEM:
    DEF_GDT(
        0x0b8000,
        0xfffff,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_S |
            GDT_ACCESS_BYTE_RW | GDT_ACCESS_BYTE_DPL3,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   /* VGA memory, 0x50 */
GDT_TSS:
    /* need to reset with real base */
    DEF_GDT(
        0x0,
        tss_len,
        GDT_ACCESS_BYTE_P | GDT_ACCESS_BYTE_E |
            GDT_ACCESS_BYTE_A,
        GDT_FLAGS_G | GDT_FLAGS_DB
    )                   /* TSS, 0x58 */

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
    )               /* ldt for test, 0x0 */

/************************************************************
* Global stack in protect mode, the size is 64 bytes.      *
*************************************************************/
    .align 32
    .code32
global_stack_section:
    .fill 4096, 1, 0
    .set global_stack_section_len, (. - global_stack_section)

/************************************************************
* Global stack in protect mode / r3, the size is 32 bytes. *
*************************************************************/
    .align 32
r3_stack_section:
    .fill 32, 1, 0
    .set r3_stack_section_len, (. - r3_stack_section)


    .align 32
TSS_SECTION:
    DEF_TSS(global_stack_section_len, GDT_SELECTOR_STACK)
    .set tss_len, (. - TSS_SECTION)



kernel_filename:
    .ascii "KERNEL  BIN"
init_loader_msg:
    .asciz "Init loader ...\r\n"
init_GDT_msg:
    .asciz "Init GDT ...\r\n"
