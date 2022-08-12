#ifndef PROTECT_H_
#define PROTECT_H_

#include <type.h>

/*
 * Present bit. Allows an entry to refer to a valid segment.
 * Must be set (1) for any valid segment.
 */
#define GDT_PRES   0x80
/*
 * Descriptor privilege level field. Contains the CPU Privilege
 * level of the segment. 0 = highest privilege (kernel),
 * 3 = lowest privilege (user applications).
 */
#define GDT_DPL_MASK 0x60
#define GDT_DPL0 0x00
#define GDT_DPL1 0x20
#define GDT_DPL2 0x40
#define GDT_DPL3 0x60
/*
 * Descriptor type bit. If clear (0) the descriptor defines
 * a system segment (eg. a Task State Segment). If set (1)
 * it defines a code or data segment.
 */
#define GDT_NOTSYS   0x10
/*
 * Executable bit. If clear (0) the descriptor defines a data
 * segment. If set (1) it defines a code segment which can be
 * executed from.
 */
#define GDT_EXE   0x08
/*
 * Direction bit/Conforming bit.
 *  - For data selectors: Direction bit. If clear (0) the segment
 *    grows up. If set (1) the segment grows down, ie. the Offset
 *    has to be greater than the Limit.
 *  - For code selectors: Conforming bit.
 *    - If clear (0) code in this segment can only be executed
 *      from the ring set in DPL.
 *    - If set (1) code in this segment can be executed from an
 *      equal or lower privilege level. For example, code in ring
 *      3 can far-jump to conforming code in a ring 2 segment.
 *      The DPL field represent the highest privilege level that
 *      is allowed to execute the segment. For example, code in
 *      ring 0 cannot far-jump to a conforming code segment where
 *      DPL is 2, while code in ring 2 and 3 can. Note that the
 *      privilege level remains the same, ie. a far-jump from ring
 *      3 to a segment with a DPL of 2 remains in ring 3 after the
 *      jump.
 */
#define GDT_DC  0x04
/*
 * Readable bit/Writable bit.
 *  - For code segments: Readable bit. If clear (0), read access for
 *    this segment is not allowed. If set (1) read access is allowed.
 *    Write access is never allowed for code segments.
 *  - For data segments: Writeable bit. If clear (0), write access for
 *    this segment is not allowed. If set (1) write access is allowed.
 *    Read access is always allowed for data segments.
 */
#define GDT_RW  0x02
/*
 * Accessed bit. Best left clear (0), the CPU will set it when the
 * segment is accessed.
 */
#define GDT_ACC   0x01

/*
 * Granularity flag, indicates the size the Limit value is scaled by.
 * If clear (0), the Limit is in 1 Byte blocks (byte granularity).
 * If set (1), the Limit is in 4 KiB blocks (page granularity).
 */
#define GDT_PG         0x80
/*
 * Size flag. If clear (0), the descriptor defines a 16-bit protected
 * mode segment. If set (1) it defines a 32-bit protected mode segment.
 * A GDT can have both 16-bit and 32-bit selectors at once.
 */
#define GDT_32BIT        0x40
/*
 * Long-mode code flag. If set (1), the descriptor defines a 64-bit code
 * segment. When set, DB should always be clear. For any other type of
 * segment (other code types or any data segment), it should be clear (0).
 */
#define GDT_LONG         0x20
// #define GDT_FLAGS_RESERVED  0x10


#define SELECTOR_GDT     0x0
#define SELECTOR_LDT     0x4
#define SELECTOR_RPL0     0x0
#define SELECTOR_RPL1     0x1
#define SELECTOR_RPL2     0x2
#define SELECTOR_RPL3     0x3


#define CALL_GATE_16    0x4
#define CALL_GATE_32    0xc
#define CALL_GATE_PL0   0x0
#define CALL_GATE_PL1   0x1
#define CALL_GATE_PL2   0x2
#define CALL_GATE_PL3   0x3
#define CALL_GATE_P     0x1
#define CALL_GATE_NP    0x0

#define GDT_NULL_IDX       0
#define GDT_CODE_IDX       1
#define GDT_DATA_IDX       2
#define GDT_TSS_IDX        3
#define GDT_LDT_IDX(n)     ((n) + 4)

#define GDT_SELECTOR_NULL       (GDT_NULL_IDX << 3)
#define GDT_SELECTOR_CODE       (GDT_CODE_IDX << 3)
#define GDT_SELECTOR_DATA       (GDT_DATA_IDX << 3)
#define GDT_SELECTOR_TSS        (GDT_TSS_IDX << 3)
#define GDT_SELECTOR_LDT(n)     (GDT_LDT_IDX(n) << 3)


#define INT_GATE_TASK       0x05
#define INT_GATE_INT16      0x06
#define INT_GATE_TRAP16     0x07
#define INT_GATE_INT32      0x0e
#define INT_GATE_TRAP32     0x0f
#define INT_GATE_PL0        0x00
#define INT_GATE_PL1        0x20
#define INT_GATE_PL2        0x40
#define INT_GATE_PL3        0x60
#define INT_GATE_P          0x80



struct gdt
{
    uint16_t limit0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t access_byte;
    uint8_t limit16_19_flags;
    uint8_t base24_31;
};


struct gdtr
{
    uint16_t size;
    uint16_t base_low;
    uint16_t base_high;
};


struct ldt
{
    uint16_t limit0_15;
    uint16_t base0_15;
    uint8_t base16_23;
    uint8_t access_byte;
    uint8_t limit16_19_flags;
    uint8_t base24_31;
};


struct tss 
{
    uint32_t back_link;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1;
    uint32_t ss1;
    uint32_t esp2;
    uint32_t ss2;
    uint32_t cr3;
    uint32_t eip;
    uint32_t eflags;
    uint32_t eax;
    uint32_t ecx;
    uint32_t edx;
    uint32_t ebx;
    uint32_t esp;
    uint32_t ebp;
    uint32_t esi;
    uint32_t edi;
    uint32_t es;
    uint32_t cs;
    uint32_t ss;
    uint32_t ds;
    uint32_t fs;
    uint32_t gs;
    uint32_t ldtr;
    uint16_t trap;
    uint16_t iobase;
};


struct idt
{
    uint16_t offset0_15;
    uint16_t selector;
    uint8_t zero;
    uint8_t attr;
    uint16_t offset16_31;
};


struct idtr
{
    uint16_t size;
    uint16_t base_low;
    uint16_t base_high;
};


/*
 * We will create 1024 page dir, each page dir point to 1024 page table
 * thus, we have 1024 * 1024 page table totally.
 * The page size is 4KB.
 */
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


void init_gdt(
    struct gdt *gdt_ptr, uint32_t base, uint32_t limit,
    uint8_t access, uint8_t flags
);
void init_ldt(
    struct gdt *gdt_ptr, uint32_t base, uint32_t limit,
    uint8_t access, uint8_t flags
);
void init_idt(
    struct idt *idt_ptr, uint32_t offset, uint16_t selector,
    uint8_t attr
);
void init_tss(struct tss *tss_ptr, uint32_t stack_top, uint32_t selector);
void setup_gdtr();
void setup_idtr();

#endif // PROTECT_H_
