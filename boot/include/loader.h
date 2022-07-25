#ifndef LOADER_H_
#define LOADER_H_

// Present bit. Allows an entry to refer to a valid segment.
// Must be set (1) for any valid segment.
#define GDT_ACCESS_BYTE_P   0x80
// Descriptor privilege level field. Contains the CPU Privilege
// level of the segment. 0 = highest privilege (kernel),
// 3 = lowest privilege (user applications).
#define GDT_ACCESS_BYTE_DPL0 0x00
#define GDT_ACCESS_BYTE_DPL1 0x20
#define GDT_ACCESS_BYTE_DPL2 0x40
#define GDT_ACCESS_BYTE_DPL3 0x60
// Descriptor type bit. If clear (0) the descriptor defines
// a system segment (eg. a Task State Segment). If set (1)
// it defines a code or data segment.
#define GDT_ACCESS_BYTE_S   0x10
// Executable bit. If clear (0) the descriptor defines a data
// segment. If set (1) it defines a code segment which can be
// executed from.
#define GDT_ACCESS_BYTE_E   0x08
// Direction bit/Conforming bit.
//  - For data selectors: Direction bit. If clear (0) the segment
//    grows up. If set (1) the segment grows down, ie. the Offset
//    has to be greater than the Limit.
//  - For code selectors: Conforming bit.
//    - If clear (0) code in this segment can only be executed
//      from the ring set in DPL.
//    - If set (1) code in this segment can be executed from an
//      equal or lower privilege level. For example, code in ring
//      3 can far-jump to conforming code in a ring 2 segment.
//      The DPL field represent the highest privilege level that
//      is allowed to execute the segment. For example, code in
//      ring 0 cannot far-jump to a conforming code segment where
//      DPL is 2, while code in ring 2 and 3 can. Note that the
//      privilege level remains the same, ie. a far-jump from ring
//      3 to a segment with a DPL of 2 remains in ring 3 after the
//      jump.
#define GDT_ACCESS_BYTE_DC  0x04
// Readable bit/Writable bit.
//  - For code segments: Readable bit. If clear (0), read access for
//    this segment is not allowed. If set (1) read access is allowed.
//    Write access is never allowed for code segments.
//  - For data segments: Writeable bit. If clear (0), write access for
//    this segment is not allowed. If set (1) write access is allowed.
//    Read access is always allowed for data segments.
#define GDT_ACCESS_BYTE_RW  0x02
// Accessed bit. Best left clear (0), the CPU will set it when the
// segment is accessed.
#define GDT_ACCESS_BYTE_A   0x01


// Granularity flag, indicates the size the Limit value is scaled by.
// If clear (0), the Limit is in 1 Byte blocks (byte granularity).
// If set (1), the Limit is in 4 KiB blocks (page granularity).
#define GDT_FLAGS_G         0x80
// Size flag. If clear (0), the descriptor defines a 16-bit protected
// mode segment. If set (1) it defines a 32-bit protected mode segment.
// A GDT can have both 16-bit and 32-bit selectors at once.
#define GDT_FLAGS_DB        0x40
// Long-mode code flag. If set (1), the descriptor defines a 64-bit code
// segment. When set, DB should always be clear. For any other type of
// segment (other code types or any data segment), it should be clear (0).
#define GDT_FLAGS_L         0x20
#define GDT_FLAGS_RESERVED  0x10


#define SELECTOR_ATTR_GDT     0x0
#define SELECTOR_ATTR_LDT     0x4
#define SELECTOR_ATTR_PL0     0x0
#define SELECTOR_ATTR_PL1     0x1
#define SELECTOR_ATTR_PL2     0x2
#define SELECTOR_ATTR_PL3     0x3


#define CALL_GATE_16    0x4
#define CALL_GATE_32    0xc
#define CALL_GATE_PL0   0x0
#define CALL_GATE_PL1   0x1
#define CALL_GATE_PL2   0x2
#define CALL_GATE_PL3   0x3
#define CALL_GATE_P     0x1
#define CALL_GATE_NP    0x0

#define GDT_SELECTOR_NULL       0x0
#define GDT_SELECTOR_CODE       0x8
#define GDT_SELECTOR_DATA       0x10
#define GDT_SELECTOR_LDT        0x18      // ldt
#define GDT_SELECTOR_CG         0x20      // call gate
#define GDT_SELECTOR_TASK       0x28
#define CG_SELECTOR_CODE        (0x30 | SELECTOR_ATTR_PL3)
#define GDT_SELECTOR_R3_CODE    (0x38 | SELECTOR_ATTR_PL3)
#define GDT_SELECTOR_STACK      0x40
#define GDT_SELECTOR_R3_STACK   (0x48 | SELECTOR_ATTR_PL3)
#define GDT_SELECTOR_VGA_MEM    (0x50 | SELECTOR_ATTR_PL3)
#define GDT_SELECTOR_TSS        0x58
#define GDT_SELECTOR_PG_DIR     0x60
#define GDT_SELECTOR_PG_TBL     0x68


#define LDT_SELECTOR_CODE (0x0 | SELECTOR_ATTR_LDT)


// we will create 1024 page dir, each page dir point to 1024 page table
// thus, we have 1024 * 1024 page table totally.
// The page size is 4KB.
#define PAGE_DIR_BASE 0x200000  // the base of page dir
#define PAGE_TBL_BASE 0x201000  // the base of page table


// present
#define PG_P        0x01
// read/write
#define PG_RW_RW    0x02
// read only
#define PG_RW_RO    0x00
// user
#define PG_US_U     0x04
// supervisor
#define PG_US_S     0x00
// page write throught, wite back otherwise
#define PG_PWT      0x00
// cache disabled if set, enabled otherwise
#define PG_PCD      0x00
// accessed, whether a PDE or PTE was read during virtual address translation.
#define PG_A        0x00
// dirty, whether a page has been written to.
#define PG_D        0x00
// page size, if the bit is set, then the PDE maps to a page that is 4 MiB in
// size. Otherwise, it maps to a 4 KiB page table.
#define PG_PS       0x00
// Page Attribute Table, If PAT is supported, then PAT along with PCD and PWT
// shall indicate the memory caching type. Otherwise, it is reserved and must
// be set to 0.
#define PG_PAT      0x00
// Global tells the processor not to invalidate the TLB entry corresponding to
// the page upon a MOV to CR3 instruction.
#define PG_G        0x00


#ifdef __ASSEMBLER__

#define DEF_GDT_NULL    \
    .word 0, 0;         \
    .byte 0, 0, 0, 0

#define DEF_GDT(base, limit, access, flags)        \
    .word (limit & 0xffff), (base & 0xffff);       \
    .byte ((base >> 16) & 0xff),                      \
          ((access) & 0xff),                            \
          (((limit >> 16) & 0x0f) | ((flags) & 0xf0)),  \
          ((base >> 24) & 0xff)


#define DEF_LDT(base, limit, access, flags)         \
    DEF_GDT(base, limit, access, flags)


#define DEF_CALL_GATE(offset, selector, param_cnt, type, dpl, p)    \
    .word (offset & 0xffff);                                        \
    .word selector;                                                 \
    .byte (param_cnt & 0x0f);                                       \
    .byte ((type & 0x0f) | ((dpl << 5) & 0x60) | ((p << 7) & 0x80));\
    .word ((offset >> 16) & 0xffff)

// 80386 32-Bit Task State Segment
#define DEF_TSS(stack_top, stack_selector)          \
    .int 0;                 /*BACK         | 0x00*/ \
    .int stack_top;         /*ESP0         | 0x04*/ \
    .int stack_selector;    /*SS0          | 0x08*/ \
    .int 0;                 /*ESP1         | 0x0c*/ \
    .int 0;                 /*SS1          | 0x10*/ \
    .int 0;                 /*ESP2         | 0x14*/ \
    .int 0;                 /*SS1          | 0x18*/ \
    .int 0;                 /*CR3          | 0x1c*/ \
    .int 0;                 /*EIP          | 0x20*/ \
    .int 0;                 /*EFLAGS       | 0x24*/ \
    .int 0;                 /*EAX          | 0x28*/ \
    .int 0;                 /*ECX          | 0x2c*/ \
    .int 0;                 /*EDX          | 0x30*/ \
    .int 0;                 /*EBX          | 0x34*/ \
    .int 0;                 /*ESP          | 0x38*/ \
    .int 0;                 /*EBP          | 0x3c*/ \
    .int 0;                 /*ESI          | 0x40*/ \
    .int 0;                 /*EDI          | 0x44*/ \
    .int 0;                 /*ES           | 0x48*/ \
    .int 0;                 /*CS           | 0x4c*/ \
    .int 0;                 /*SS           | 0x50*/ \
    .int 0;                 /*DS           | 0x54*/ \
    .int 0;                 /*FS           | 0x58*/ \
    .int 0;                 /*GS           | 0x5c*/ \
    .int 0;                 /*LDTR         | 0x60*/ \
    .word 0;                 /*LDTR         | 0x60*/ \
    .word (. - TSS_SECTION + 2);             /*I/O MAP base | 0x64*/ \
    .byte 0xff;             /*end          | 0x68*/


#else // __ASSEMBLER__

struct gdt
{
    short limit0_15;
    short base0_15;
    unsigned char base16_23;
    unsigned char access_byte;
    unsigned char limit16_19_flags;
    unsigned char base24_31;
};

#endif // __ASSEMBLER__

#endif // LOADER_H_
