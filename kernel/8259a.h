#ifndef PIC8259A_H_
#define PIC8259A_H_

#define DISABLE_INT()   \
    __asm__ __volatile__("cli")
#define ENABLE_INT()   \
    __asm__ __volatile__("sti")


void setup_8259a();
void master_int(int id);
void slave_int(int id);
void disable_irq(int id);
void enable_irq(int id);

#endif // PIC8259A_H_