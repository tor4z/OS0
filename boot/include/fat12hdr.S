                            # Start Length Stored-data
    .ascii "OS0 Boot"       #    0  11     OEM string 8 bytes
    .short 512              #   11   2     Bytes per sector
    .byte  1                #   13   1     Sectors per cluster
    .short 1                #   14   2     Number of reserved sectors (ocuppied by boot)
    .byte  2                #   16   1     Number of FATs
    .short 224              #   17   2     Maximum number of root directory entries
    .short 2800             #   19   2     Totla sector count
    .byte  0xf0             #   21   1     Ignored, media descriptor
    .short 9                #   22   2     Sectors per FAT
    .short 18               #   24   2     Sectors per track
    .short 2                #   26   2     Number of heads
    .int   0                #   28   4     Ignore
    .int   0                #   32   4     Total sector count for FAT12
    .byte  0                #   36   1     Drive number
    .byte  0                #   37   1     Reserved
    .byte  0x29             #   38   1     Boot signature
    .int   0                #   39   4     Volume ID
    .ascii "OS0.S0     "    #   43  11     Volume label
    .ascii "FAT12   "       #   54   8     File system type
