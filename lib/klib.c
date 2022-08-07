#include <klib.h>


static void swap(char *a, char *b);
static void reverse(char *begin, char *end);


char *itoa (int value, char *str, int base)
{
    char *s = str;

    if(base == 2)
    {
        while (value)
        {
            *s = (value % 2) + '0';
            value /= 2;
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
        while (value)
        {
            *s = (value % 8) + '0';
            value /= 8;
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

        while (value)
        {
            t = (value % 16);
            if(t > 9)
                *s = t + 'a' - 10;
            else
                *s = t + '0';
            value /= 16;
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
        while (value)
        {
            *s = (value % 10) + '0';
            value /= 10;
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
