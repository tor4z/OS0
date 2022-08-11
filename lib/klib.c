#include <klib.h>
#include <type.h>


static void swap(char *a, char *b);
static void reverse(char *begin, char *end);


char *itoa (int value, char *str, int base)
{
    char *s = str;

    if(base == 2)
    {
        uint32_t v = (uint32_t)value;

        if(v == 0)
        {
            *s = '0';
            ++s;
        }

        while (v)
        {
            *s = (v % 2) + '0';
            v /= 2;
            ++s;
        }

        *s = 'b';
        ++s;
        *s = '0';

        reverse(str, s);
        ++s;
        *s = '\0';
    }
    else if(base == 8)
    {
        uint32_t v = (uint32_t)value;

        while (v)
        {
            *s = (v % 8) + '0';
            v /= 8;
            ++s;
        }
        
        *s = '0';

        reverse(str, s);
        ++s;
        *s = '\0';
    }
    else if(base == 16)
    {
        int t;
        uint32_t v = (uint32_t)value;

        if (v == 0)
        {
            *s = '0';
            ++s;
        }

        while (v)
        {
            t = (v % 16);
            if(t > 9)
                *s = t + 'a' - 10;
            else
                *s = t + '0';
            v /= 16;
            ++s;
        }

        *s = 'x';
        ++s;
        *s = '0';

        reverse(str, s);
        ++s;
        *s = '\0';
    }
    else if (base == 10)
    {
        int negtive = 0;

        if(value < 0)
        {
            negtive = 1;
            value *= -1;
        }

        if (value == 0)
        {
            *s = 0;
            ++s;
        }

        while (value)
        {
            *s = (value % 10) + '0';
            value /= 10;
            ++s;
        }


        if (negtive)
        {
            *s = '-';
            ++s;
        }

        --s;
        reverse(str, s);
        ++s;
        *s = '\0';
    }

    return str;
}


static void swap(char *a, char *b)
{
    char t = *a;
    *a = *b;
    *b = t;
}


static void reverse(char *begin, char *end)
{
    while (begin < end)
    {
        swap(begin, end);
        ++begin;
        --end;
    }
}
