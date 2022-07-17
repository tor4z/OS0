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

    ################
    # clean screen #
    ################
    # ah: function code
    # al: number of line to scroll, 00h for entire window
    # ch, cl: row, col of window's upper left corner
    # dh, dl: row, col of window's lower right corner
    movw $0x0600, %ax       # Set ah to 06h, set al to 00h
    movw $0x0700, %bx       # Set font color, and background color
    movw $0, %cx            # Set upper left corner to (0, 0)
    movw $0x1850, %dx       # Set lower right corner to (80, 24)
    int $0x10               # interrupt

    leaw init_msg, %si
    call print_string
fin:
    hlt
    jmp fin




################################################################
# @fnnction: print a string                                    #
# @parameter:                                                  #
#   si: The address of the string, the string MUST end with 0. #
# @return                                                      #
#   nothing                                                    #
################################################################
    .type print_string, @function
print_string:
    movb (%si), %al         # load the char into al register
    incw %si                # increse si, try to read next char
    cmpb $0, %al            # Check is reach the end of the string
    je print_string_ret     # Jump to return if we reach the end of the string
    movb $0x0e, %ah         # Set 0x0e to ah means print a char
    movw $10, %bx           # Set page number and color
    int $0x10               # interrupt
    jmp print_string        # print next char
print_string_ret:
    ret                     # return


init_msg:
    .asciz "Init boot...\n"
read_success_msg:
    .asciz "Read disk success\n"
read_fail_msg:
    .asciz "Read disk fail\n"
jump_to_loader_msg:
    .asciz "Ready to jump to loader\n"


    .fill 510 - (. - boot_init), 1, 0
    .byte 0x55, 0xaa
