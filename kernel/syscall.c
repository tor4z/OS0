#include <sys/const.h>
#include <sys/syscall.h>


int get_ticks()
{
    int result;
    __asm__ __volatile__(
        "int %2"
        : "=a"(result)
        : "a"(SYSCALL_GET_TICKS), "i"(INT_VEC_SYSCALL)
        :
    );

    return result;
}
