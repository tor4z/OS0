.set BaseOfStack, 0x7c00

    .code16
boot_init:
    ###################
    # Floppy settings #
    ###################
    jmp boot_entry
    nop
    .include "include/fat12hdr.S"

boot_entry:
    ################################################
    # We setting up the ds, es, ss and sp register #
    ################################################
    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    movw $BaseOfStack, %sp

    call clean_screen
    call reset_sector
    call read_FAT1
read_loader:
    call find_loader
    # the start logical cluster is stored in ax register
    call read_file
    jmp 0x9200


################################################################
# @function: read FAT1                                         #
# @parameter:                                                  #
#   Nothing                                                    #
# @return                                                      #
#   Nothing                                                    #
################################################################
    .set FAT1_ADDR, 0x7e00
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

################################################################
# @function: query FAT1                                        #
# @parameter:                                                  #
#   ax: logical cluster                                       #
# @return                                                      #
#   ax: next cluster or EOC                                    #
################################################################
    .type query_FAT1, @function
query_FAT1:
    pushw %bx
    pushw %cx
    pushw %si

    movw %ax, %cx
    movw $3, %bx
    mulw %bx
    movw $2, %bx
    divw %bx
    addw $FAT1_ADDR, %ax
    movw %ax, %si
    movb (%si), %bl
    inc %si
    movb (%si), %bh
    testw $0x1, %cx
    jz cluster_number_even
    jmp cluster_number_odd
cluster_number_even:
    xorw %ax, %ax
    movb %bh, %ah
    movb %bl, %al
    andw $0xfff, %ax
    jmp query_FAT1_ret
cluster_number_odd:
    xorw %ax, %ax
    movb %bl, %al
    movb %bh, %ah
    shrw $4, %ax
    andw $0xfff, %ax
query_FAT1_ret:
    popw %si
    popw %cx
    popw %bx
    ret

################################################################
# @function: read file                                         #
# @parameter:                                                  #
#   ax: begin sector                                           #
#   es:di: target address                                      #
# @return                                                      #
#   Nothing                                                    #
################################################################
    .type read_file, @function
read_file:
    pushw %bx
    pushw %cx
    pushw %dx

    movw $LOADER_ADDR, %bx
read_file_loop:
    movw %ax, %dx
    call logical_to_physical
    movw $1, %cx
    call read_sector
    movw %dx, %ax
    call query_FAT1
    cmpw $0xfff, %ax
    je read_file_ret
    addw $512, %bx
    jmp read_file_loop
read_file_ret:
    popw %dx
    popw %cx
    popw %bx
    ret


################################################################
# @function: find file                                         #
# @parameter:                                                  #
#   Nothing                                                    #
# @return                                                      #
#   Nothing                                                    #
################################################################
# Temporary memory to store direction (512B)
    .set TEMP_MEM, 0x9000

    .type find_file, @function
find_loader:
    movw $ROOT_DIR_BEGIN_SECTOR_INDEX, %ax
find_loader_loop:
    movw $1, %cx                    # We read 1 sector in a times
    movw $TEMP_MEM, %bx
    call read_sector

    movw $TEMP_MEM, %di
    movw $NUM_DIR_ENTRY_A_SECTOR, %dx
find_loader_cmp:
    leaw loader_filename, %si
    movw $11, %cx
    cld
    repe cmpsb
    je find_loader_ret
    addw $32, %di
    subw $loader_filename, %si
    subw %si, %di
    dec %dx
    cmpw $0, %dx
    jnz find_loader_cmp
    inc %ax
    cmpw $MAX_ROOT_DIR_SECTOR_INDEX, %ax
    jnc find_loader_loop
find_loader_error:
    leaw loader_notfound_msg, %si
    call print_string
find_loader_ret:
    addw $15, %di
    movw (%di), %ax
    ret


// init_msg:
//     .asciz "Init ...\r\n"
read_success_msg:
    .asciz "Read success\r\n"
loader_notfound_msg:
    .asciz "Loader notfound\r\n"
FAT1_loaded_msg:
    .asciz "FAT1 loaded\r\n"

    .include "include/boothdr.S"

    .fill 510 - (. - boot_init), 1, 0
    .byte 0x55, 0xaa
