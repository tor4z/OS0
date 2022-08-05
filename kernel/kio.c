#include <kernel/vga.h>
#include <kernel/kio.h>


int32_t putchar(char c)
{
    write_screen(c);
    return (int)c;
}


int32_t puts(const char *str)
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
