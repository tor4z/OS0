#ifndef CONST_H_
#define CONST_H_

#define NUM_GDT 128
#define NUM_IDT 256

#define MASTER_8259A_P0 0x20
#define SLAVE_8259A_P0  0xa0
#define MASTER_8259A_P1 0x21
#define SLAVE_8259A_P1  0xa1

#define IRQ_MASTER_VEC  0x20
#define IRQ_SLAVE_VEC   0x28

#endif // CONST_H_
