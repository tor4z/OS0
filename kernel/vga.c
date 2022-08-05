#include <kernel/vga.h>


static uint32_t scrn_z0 = 0x22222222;
static uint32_t scrn_x = 0;
static uint32_t scrn_y = 0;
static uint32_t scrn_z1 = 0x11111111;

static void scroll_line();
static void write_newline();


void clean_screen()
{
    uint32_t total = SCRN_H * SCRN_W;        // the number of data to fill

    __asm__ __volatile__(
        "movw $0x0720, %%ax\n\t"        // the data to fill
        "movl %0, %%edi\n\t"            // set target address es:(e)di
        "rep stosw"
        : /*no output*/
        : "i"(VGA_ADDR), "c"(total)
        : "memory", "eax", "edi"
    );
}


void write_screen(char c)
{
    write_screen_xy(c, scrn_x, scrn_y);
}


void write_screen_xy(char c, uint32_t x, uint32_t y)
{
    if (c == '\n')
    {
        write_newline();
        return;
    }

    uint32_t position = VGA_ADDR + (y * SCRN_W + x) * 2;
    uint16_t data = c | 0x0700;

    __asm__ __volatile__(
        "movw %%ax, (%%edi)"
        : /*no output*/
        : "a"(data), "D"(position)
        : "memory"
    );

    ++x;
    if(x >= SCRN_W)
    {
        x = 0;
        ++y;
    }

    if(y >= SCRN_H)
    {
        scroll_line();
        --y;
    }

    scrn_x = x;
    scrn_y = y;
}


static void scroll_line()
{
    uint16_t cnt = SCRN_W * (SCRN_H - 1);
    uint32_t src = VGA_ADDR + SCRN_W * 2;
    uint32_t des = VGA_ADDR;

    __asm__ __volatile__(
        "cld\n\t"
        "rep movsw"
        : /*no output*/
        : "c"(cnt), "S"(src), "D"(des)
        : "memory", "eax"
    );
}


static void write_newline()
{
    scrn_x = 0;
    ++scrn_y;

    if(scrn_y >= SCRN_H)
    {
        scroll_line();
        --scrn_y;
    }
}
