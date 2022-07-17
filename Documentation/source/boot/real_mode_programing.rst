====================
Real mode programing
====================

------------
clean screen
------------

::

    VIDEO - SCROLL UP WINDOW
    AH = 06h
    AL = number of lines by which to scroll up (00h = clear entire window)
    BH = attribute used to write blank lines at bottom of window
    CH,CL = row,column of window's upper left corner
    DH,DL = row,column of window's lower right corner


------------
Print a char
------------



::

    VIDEO - TELETYPE OUTPUT
    AH = 0Eh
    AL = character to write
    BH = page number
    BL = foreground color (graphics modes only)

    Return:
    Nothing

    Desc: Display a character on the screen, advancing the cursor
    and scrolling the screen as necessary

    Color control:
    The attribute byte carries the foreground colour in its
    lowest 4 bits and the background color in its highest 3
    bits. For instance, using 0x00 as attribute byte means
    black-on-black (you'll see nothing). 0x07 is lightgrey-on-black
    (DOS default), 0x1F is white-on-blue (Win9x's blue-screen-of-death),
    0x2a is for green-monochrome nostalgics.

