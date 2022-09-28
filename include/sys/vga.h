#ifndef VGA_H_
#define VGA_H_

#include <type.h>


#define VGA_ADDR 0xb8000
#define SCRN_W 80
#define SCRN_H 25


void clean_screen();
void write_screen(char c);
void write_screen_xy(char c, uint32_t x, uint32_t y);


#endif // VGA_H_
