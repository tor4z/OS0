    .set ROOT_DIR_SECTORS, 14
    .set ROOT_DIR_BEGIN_SECTOR_INDEX, 19
    .set MAX_ROOT_DIR_SECTOR_INDEX, 33
    .set NUM_DIR_ENTRY_A_SECTOR, 16
    .set FAT1_ADDR, 0x7e00

    .set VIDEO_MEM,   0xb8000
    .set SCRN_W_ADDR, 0xa000    /* To store max screen cloume (4 bytes) */
    .set SCRN_H_ADDR, 0xa004    /* To store max screen row (4 bytes) */
    .set SCRN_X_ADDR, 0xa008    /* To store current screen cloume (4 bytes) */
    .set SCRN_Y_ADDR, 0xa00c    /* To store current screen row (4 bytes) */

    .set LOADER_ADDR, 0x9200
    .set KERNEL_ADDR, 0xb000

