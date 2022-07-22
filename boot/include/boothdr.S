    .set LOADER_ADDR, 0x9200


################################################################
# @function: clean screen                                      #
# @parameter:                                                  #
#   Nothing                                                    #
# @return                                                      #
#   Nothing                                                    #
################################################################
    .type clean_screen, @function
clean_screen:
    # ah: function code
    # al: number of line to scroll, 00h for entire window
    # ch, cl: row, col of window's upper left corner
    # dh, dl: row, col of window's lower right corner
    pushw %ax
    pushw %bx
    pushw %cx
    pushw %dx
    movw $0x0600, %ax       # Set ah to 06h, set al to 00h
    movw $0x0700, %bx       # Set font color, and background color
    movw $0x0, %cx          # Set upper left corner to (0, 0)
    movw $0x1850, %dx       # Set lower right corner to (80, 24)
    int $0x10               # interrupt
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


################################################################
# @fnnction: print a string                                    #
# @parameter:                                                  #
#   si: The address of the string, the string MUST end with 0. #
# @return                                                      #
#   Nothing                                                    #
################################################################
    .type print_string, @function
print_string:
    pushw %bx
    pushw %ax
print_string_loop:
    movb (%si), %al         # load the char into al register
    incw %si                # increse si, try to read next char
    cmpb $0, %al            # Check is reach the end of the string
    jz print_string_ret     # Jump to return if we reach the end of the string
    movb $0x0e, %ah         # Set 0x0e to ah means print a char
    movw $10, %bx           # Set page number and color
    int $0x10               # interrupt
    jmp print_string_loop   # print next char
print_string_ret:
    popw %ax
    popw %bx
    ret                     # return


################################################################
# @function reset sector                                       #
# @parameter:                                                  #
#   Nothing                                                    #
# @return                                                      #
#   Noting                                                     #
################################################################
    .type reset_sector, @function
reset_sector:
    // leaw reset_msg, %si
    // call print_string

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

################################################################
# @function read sector                                        #
# @parameter:                                                  #
#   ax: the sector index begin to read                         #
#   cx: the number of sectors to read                          #
#   es:bx: the data to be store                                #
# @return                                                      #
#   Noting                                                     #
################################################################
    .type read_sector, @function
read_sector:
    // leaw read_sector_msg, %si
    // call print_string

    pushw %bx
    pushw %dx
    pushw %si
    pushw %di

    movw %cx, %si
    movw %ax, %di
read_sector_loop:
    movw %di, %ax           # Get the current sector number
    movb $18, %cl
    divb %cl                # Calculate sectror number
    movb %ah, %cl           # The sector number
    inc %cl                 # cl += 1
    movb %al, %dh           # Ready to calculate head number
    andb $0x1, %dh          # The head number
    movb %al, %ch           # Ready to calculate cylinder number
    shrb %ch                # The cylinder number

    movb $0x02, %ah         # Read sector function number
    movb $0, %dl            # The drive number
    movb $1, %al            # Read 1 sector once
    int $0x13               # Interrupt
    jc read_sector_error    # Jump to error on read sector error

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
    popw %bx
    ret

###############################################################
# @function conver logical cluster NO. to physical cluster NO.# 
# @parameter                                                  #
#   ax: the logical cluster                                   #
# @return                                                     #
#   ax: the physical cluster                                  #
###############################################################
    .type logical_to_physical, @function
logical_to_physical:
    addw $31, %ax
logical_to_physical_ret:
    ret


reset_fail_msg:
    .asciz "Reset fail\r\n"
// reset_msg:
//     .asciz "Reset disk\r\n"
// read_sector_msg:
//     .asciz "Read sector\r\n"
read_fail_msg:
    .asciz "Read fail\r\n"
loader_filename:
    .ascii "LOADER  BIN"
