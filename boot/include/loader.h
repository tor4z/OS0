#ifndef LOADER_H_
#define LOADER_H_

// Present bit. Allows an entry to refer to a valid segment.
// Must be set (1) for any valid segment.
#define GDT_ACCESS_BYTE_P   0x80
// Descriptor privilege level field. Contains the CPU Privilege
// level of the segment. 0 = highest privilege (kernel),
// 3 = lowest privilege (user applications).
#define GDT_ACCESS_BYTE_DPL 0x60
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
