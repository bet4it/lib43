#include <sys/syscall.h>
#include "syscall.h"

arg syscall(int n, arg _1, arg _2, arg _3, arg _4, arg _5, arg _6) {
    SYSCALL_PREFIX
    __asm__ __volatile__(
        SYSCALL_ASM
        SYSCALL_SUFFIX
    );
    SYSCALL_RET
}
