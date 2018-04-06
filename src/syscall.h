#ifndef SYSCALL_H
#define SYSCALL_H

#include "errno.h"

#include <stdint.h>
#include <sys/syscall.h>

#if defined(__APPLE__) && defined(__LP64__)
#  define SYS(name) (SYS_##name + 0x2000000)
#else
#  define SYS(name) SYS_##name
#endif

#if defined(__x86_64)
typedef int64_t abi_long;
#elif defined(__i386__) || defined(__mips__) || defined(__arm__)
typedef int32_t abi_long;
#endif

extern abi_long syscall(int n, abi_long a1, abi_long a2, abi_long a3, abi_long a4, abi_long a5, abi_long a6);

#define syscall6(n, a1, a2, a3, a4, a5, a6)                        \
    (abi_long)syscall(n, (abi_long)a1, (abi_long)a2, (abi_long)a3, \
                         (abi_long)a4, (abi_long)a5, (abi_long)a6)
#define syscall5(n, args...) syscall6(n, args, 0)
#define syscall4(n, args...) syscall5(n, args, 0)
#define syscall3(n, args...) syscall4(n, args, 0)
#define syscall2(n, args...) syscall3(n, args, 0)
#define syscall1(n, args...) syscall2(n, args, 0)
#define syscall0(n) syscall1(n, 0)

#define reg(name) register abi_long name __asm__(#name)
#if defined(__linux__) || defined(__APPLE__)
#  ifdef __x86_64__
#    define SYSCALL_PREFIX reg(rax) = n; reg(rdi) = _1; reg(rsi) = _2; reg(rdx) = _3; \
                                         reg(r10) = _4; reg(r8)  = _5; reg(r9)  = _6;
#    define SYSCALL_ASM "syscall;"
#    define SYSCALL_SUFFIX :"=r"(rax)                               \
                           :"r"(rax), "r"(rdi), "r"(rsi), "r"(rdx), \
                                      "r"(r10), "r"(r8),  "r"(r9)   \
                           :"rcx", "r11"
#    define SYSCALL_RET return rax;
#  elif defined(__i386__)
#    define SYSCALL_PREFIX reg(eax) = n;
#    define SYSCALL_ASM "int $0x80;"
#    define SYSCALL_SUFFIX :"=r"(eax) \
                           :"r"(eax), "b"(_1), "c"(_2), "d"(_3), "S"(_4), "D"(_5)
#    define SYSCALL_RET return eax;
#  elif defined(__mips__)
#    define SYSCALL_PREFIX reg(v0) = n; reg(a0) = _1; reg(a1) = _2; reg(a2) = _3; \
                                        reg(a4) = _4; reg(a5) = _5; reg(a6) = _6;
#    define SYSCALL_ASM "syscall;"
#    define SYSCALL_SUFFIX :"=r"(v0)
#    define SYSCALL_RET return v0;
#  endif
#endif

#ifndef SYSCALL_ASM
#  error "Unsupported OS/arch."
#endif

#define arg abi_long
#endif // SYSCALL_H
