    .set BaseOfStack, 0x7c00

    .code16
    .globl _start
_start:
    /*****************************
     *    Floppy settings        *
     *****************************/
    jmp boot_entry
    nop
    .include "include/fat12hdr.S"
boot_entry:
    /************************************************
     * We setting up the ds, es, ss and sp register *
     ************************************************/
    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    movw $BaseOfStack, %sp

    call clean_screen
    call reset_sector
    call read_FAT1
read_loader:
    leaw loader_filename, %ax
    call find_file
    // the start logical cluster is stored in ax register
    movw $LOADER_ADDR, %bx
    call read_file
    jmp LOADER_ADDR


/*******************************************************
 * @function: read FAT1                                *
 * @parameter:                                         *
 *   Nothing                                           *
 * @return                                             *
 *   Nothing                                           *
 *******************************************************/
    .code16
    .type read_FAT1, @function
read_FAT1:
    pushw %ax
    pushw %cx

    movw $1, %ax
    movw $9, %cx
    movw $FAT1_ADDR, %bx
    call read_sector
read_FAT1_ret:
    popw %cx
    popw %ax
    leaw FAT1_loaded_msg, %si
    call print_string
    ret


loader_filename:
    .ascii "LOADER  BIN"
read_success_msg:
    .asciz "Read success\r\n"
FAT1_loaded_msg:
    .asciz "FAT1 loaded\r\n"

    .include "include/boothdr.S"
    .include "include/def.S"

    .fill 510 - (. - _start), 1, 0
    .byte 0x55, 0xaa
