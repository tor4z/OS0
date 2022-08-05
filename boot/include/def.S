    .set ROOT_DIR_SECTORS,              14
    .set ROOT_DIR_BEGIN_SECTOR_INDEX,   19
    .set MAX_ROOT_DIR_SECTOR_INDEX,     33
    .set NUM_DIR_ENTRY_A_SECTOR,        16
    .set FAT1_ADDR,                     0x7e00

    .set VIDEO_MEM,   0xb8000
    .set SCRN_W_ADDR, 0xa100    /* To store max screen cloume (4 bytes) */
    .set SCRN_H_ADDR, 0xa104    /* To store max screen row (4 bytes) */
    .set SCRN_X_ADDR, 0xa108    /* To store current screen cloume (4 bytes) */
    .set SCRN_Y_ADDR, 0xa10c    /* To store current screen row (4 bytes) */
    .set KERNEL_SIZE, 0xa110    /* Kernel size in sectors (4 bytes) */

    .set LOADER_ADDR,       0x9200
    .set KERNEL_TMP_ADDR,   0xa200
    .set KERNEL_ADDR,       0x30000
    .set KERNEL_ENTRY,      0x30400

    /* ELF */
    .set ELF_PHNUM_OFFSET,      0x2c
    .set ELF_SHOFF_OFFSET,      0x20
    .set ELF_SHNUM_OFFSET,      0x30

    /* the offset of first program header */
    .set ELF_PH_OFFSET,         0x34
    .set ELF_PH_TYPE_OFFSET,    0x00
    .set ELF_PH_OFFSET_OFFSET,  0x04
    .set ELF_PH_VADDR_OFFSET,   0x08
    .set ELF_PH_PADDR_OFFSET,   0x0c
    .set ELF_PH_FILESZ_OFFSET,  0x10
    .set ELF_PH_MEMSZ_OFFSET,   0x14
    .set ELF_PH_FLAGS_OFFSET,   0x18
    .set ELF_PH_ALIGN_OFFSET,   0x1c
    .set ELF_PH_SIZE,           0x20

    .set ELF_SH_TYPE_OFFSET,    0x04
    .set ELF_SH_ADDR_OFFSET,    0x0c
    .set ELF_SH_SIZE_OFFSET,    0x14
    .set ELF_SH_SIZE,           0x28

    .set ELF_PT_LOAD,           0x00000001
    .set ELF_SHT_NOBITS,        0x00000008
