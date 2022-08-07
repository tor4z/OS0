#include <kernel/vga.h>
#include <kernel/kio.h>
#include <kernel/type.h>


int32_t kputchar(char c)
{
    write_screen(c);
    return (int)c;
}


int32_t kputs(const char *str)
{
    while (*str)
    {
        write_screen(*str);
        ++str;
    }
    write_screen('\n');
    return 0;
    // return EOF;  // on error
}


int32_t kprint(const char *str)
{
    int32_t i = 0;
    while (*str)
    {
        write_screen(*str);
        ++str;
        ++i;
    }
    return i;
    // return -1;  // on error
}


void out_byte(uint16_t port, uint8_t data)
{
    __asm__ __volatile__(
        "outb %0, %1"
        : /* no putout */
        : "a"(data), "Nd"(port)
        :
    );
}


uint8_t in_byte(uint16_t port)
{
    uint8_t data = 0;
    __asm__ __volatile__(
        "inb %1, %0"
        : "=a"(data)
        : "Nd"(port)
        :
    );
    return data;
}
