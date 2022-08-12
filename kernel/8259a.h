#ifndef PIC8259A_H_
#define PIC8259A_H_

void setup_8259a();
void master_int(int id);
void slave_int(int id);
void disable_int(int id);
void enable_int(int id);

#endif // PIC8259A_H_