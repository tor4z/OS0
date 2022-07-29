.include "def.S"

/*****************************************************
 * @function: clean screen                           *
 * @parameter:                                       *
 *   Nothing                                         *
 * @return                                           *
 *   Nothing                                         *
 * ***************************************************/
    .code16
    .type clean_screen, @function
clean_screen:
    /* ah: function code
     * al: number of line to scroll, 00h for entire window
     * ch, cl: row, col of window's upper left corner
     * dh, dl: row, col of window's lower right corner
     */
    pushw %ax
    pushw %bx
    pushw %cx
    pushw %dx

    movw $0x0600, %ax       /* Set ah to 06h, set al to 00h */
    movw $0x0700, %bx       /* Set font color, and background color */
    movw $0x0, %cx          /* Set upper left corner to (0, 0) */
    movw $0x1850, %dx       /* Set lower right corner to (80, 24) */
    int $0x10               /* interrupt */
    movb $0x02, %ah
    movb $0x0, %bh
    movw $0x0, %dx
    int $0x10
clean_screen_ret:
    popw %dx
    popw %cx
    popw %bx
    popw %ax
    ret


/****************************************************************
 * @fnnction: print a string                                    *
 * @parameter:                                                  *
 *   si: The address of the string, the string MUST end with 0. *
 * @return                                                      *
 *   Nothing                                                    *
 ****************************************************************/
    .code16
    .type print_string, @function
print_string:
    pushw %bx
    pushw %ax
print_string_loop:
    movb (%si), %al         /* load the char into al register */
    incw %si                /* increse si, try to read next char */
    cmpb $0, %al            /* Check is reach the end of the string */
    jz print_string_ret     /* Jump to return if we reach the end of the string */
    movb $0x0e, %ah         /* Set 0x0e to ah means print a char */
    movw $10, %bx           /* Set page number and color */
    int $0x10               /* interrupt */
    jmp print_string_loop   /* print next char */
print_string_ret:
    popw %ax
    popw %bx
    ret                     /* return */


/*****************************************************
* @function reset sector                             *
* @parameter:                                        *
*   Nothing                                          *
* @return:                                           *
*   Noting                                           *
******************************************************/
    .code16
    .type reset_sector, @function
reset_sector:
    pushw %ax
    pushw %dx

    xorb %ah, %ah
    movb $0, %dl
    int $13
    jc reset_sector_error
    jmp reset_sector_ret
reset_sector_error:
    leaw reset_fail_msg, %si
    call print_string
reset_sector_ret:
    popw %dx
    popw %ax
    ret

/******************************************************
 * @function read sector                              *
 * @parameter:                                        *
 *   ax: the sector index begin to read               *
 *   cx: the number of sectors to read                *
 *   bx: the data to be store                         *
 * @return                                            *
 *   Noting                                           *
 ******************************************************/
/*         DISK - READ SECTOR(S) INTO MEMORY (int 0x13)
 * AH = 02h
 * AL = number of sectors to read (must be nonzero)
 * CH = low eight bits of cylinder number
 * CL = sector number 1-63 (bits 0-5)
 * high two bits of cylinder (bits 6-7, hard disk only)
 * DH = head number
 * DL = drive number (bit 7 set for hard disk)
 * ES:BX -> data buffer
 */  
    .code16
    .type read_sector, @function
read_sector:
    pushw %dx
    pushw %si
    pushw %di

    movw %cx, %si
    movw %ax, %di
read_sector_loop:
    movw %di, %ax           /* Get the current sector number */
    movb $18, %cl
    divb %cl                /* Calculate sectror number */
    movb %ah, %cl           /* The sector number */
    inc %cl                 /* cl += 1 */
    movb %al, %dh           /* Ready to calculate head number */
    andb $0x1, %dh          /* The head number */
    movb %al, %ch           /* Ready to calculate cylinder number */
    shrb %ch                /* The cylinder number */

    movb $0x02, %ah         /* Read sector function number */
    movb $0, %dl            /* The drive number */
    movb $1, %al            /* Read 1 sector once */
    int $0x13               /* Interrupt */
    jc read_sector_error    /* Jump to error on read sector error */

    dec %si
    inc %di
    addw $512, %bx
    cmpw $0, %si
    jnz read_sector_loop
    jz read_sector_ret
read_sector_error:
    leaw read_fail_msg, %si
    call print_string
read_sector_ret:
    popw %di
    popw %si
    popw %dx
    ret

/*****************************************************************
 * @function: conver logical cluster NO. to physical cluster NO. *
 * @parameter:                                                   *
 *   ax: the logical cluster                                     *
 * @return:                                                      *
 *   ax: the physical cluster                                    *
 *****************************************************************/
    .code16
    .type logical_to_physical, @function
logical_to_physical:
    addw $31, %ax
logical_to_physical_ret:
    ret


/******************************************************
 * @function: find file                               *
 * @parameter:                                        *
 *   ax: the address of filename                      *
 * @return                                            *
 *   ax: the start logic cluster                      *
 ******************************************************/
/* Temporary memory to store direction (512B) */
    .set TEMP_MEM, 0x9000
    .code16
    .type find_file, @function
find_file:
    pushw %bx
    pushw %dx
    pushw %cx
    pushw %di
    pushw %si

    movw $ROOT_DIR_BEGIN_SECTOR_INDEX, %ax
find_file_loop:
    movw $1, %cx                    /* We read 1 sector once */
    movw $TEMP_MEM, %bx
    call read_sector

    movw $TEMP_MEM, %di
    movw $NUM_DIR_ENTRY_A_SECTOR, %dx
find_file_cmp:
    movw %bx, %si
    movw $11, %cx
    cld
    repe cmpsb
    je find_file_ret
    addw $32, %di
    subw %bx, %si
    subw %si, %di
    dec %dx
    cmpw $0, %dx
    jnz find_file_cmp
    inc %ax
    cmpw $MAX_ROOT_DIR_SECTOR_INDEX, %ax
    jnc find_file_loop
find_file_error:
    leaw file_notfound_msg, %si
    call print_string
find_file_ret:
    addw $15, %di
    movw (%di), %ax

    popw %si
    popw %di
    popw %cx
    popw %dx
    popw %bx
    ret


/***************************************************
 * @function: read file                            *
 * @parameter:                                     *
 *   ax: begin sector                              *
 *   bx: target address                            *
 * @return                                         *
 *   Nothing                                       *
 ***************************************************/
    .code16
    .type read_file, @function
read_file:
    pushw %cx
    pushw %dx

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
    ret


/****************************************************
 * @function: query FAT1                            *
 * @parameter:                                      *
 *  ax: logical cluster                             *
 * @return                                          *
 *  ax: next cluster or EOC                         *
 ****************************************************/
    .code16
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
    movb %ds:0(%si), %bl
    inc %si
    movb %ds:0(%si), %bh
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


file_notfound_msg:
    .asciz "File notfound\r\n"
reset_fail_msg:
    .asciz "Reset fail\r\n"
read_fail_msg:
    .asciz "Read fail\r\n"
