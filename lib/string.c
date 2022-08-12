#include <string.h>


void *memcpy(void *dest, const void *src, size_t n)
{
    __asm__ __volatile__(
        "cld\n\t"
        "movl %%edx, %%ecx\n\t"
        "shrl $2, %%ecx\n\t"
        "rep movsl\n\t"
        "movl %%edx, %%ecx\n\t"
        "andl $0x03, %%ecx\n\t"
        "rep movsb\n\t"
        : /* not output */
        : "S"(src), "D"(dest), "d"(n)
        : "ecx", "memory"
    );
    return dest;
}


void *memset(void *ptr, int x, size_t n)
{
    /*
     * c − This is the value to be set. The value is passed as an
     * int, but the function fills the block of memory using the
     * unsigned char conversion of this value.
     *
     * 1. we set eax to al_al_al_al.
     * 2. we set the memory by 4-bytes block with eax.
     * 3. we set the remaining memory with al.
     */
    __asm__ __volatile__(
        "cld\n\t"
        "movb %%bl, %%al\n\t"
        "movb %%bl, %%ah\n\t"
        "shll $16, %%eax\n\t"
        "movb %%bl, %%al\n\t"
        "movb %%bl, %%ah\n\t"
        "movl %%edx, %%ecx\n\t"
        "shrl $2, %%ecx\n\t"        // number of blocks
        "rep stosl\n\t"
        "movl %%edx, %%ecx\n\t"
        "andl $0x3, %%ecx\n\t"      // number of remaining bytes
        "rep stosb"
        : /* no output */
        : "b"(x), "d"(n), "D"(ptr)
        : "eax", "ecx", "memory"
    );

    return ptr;
}


char* strcpy(char* dest, const char* src)
{
    char *ret = dest;
    while (*src)
    {
        *dest = *src;
        ++src;
        ++dest;
    }
    *dest = '\0';
    return ret;
}


size_t strlen(const char *str)
{
    size_t len = 0;
    while (*str)
    {
        ++len;
        ++str;
    }
    return len;
}

