//
// Created by o0kam1 on 2025/05/16.
//

#ifndef ARCH_H
#define ARCH_H

#ifndef UEFI_LIBC_ARCH
#if defined(__i386__) || defined(__arm__)
#define UEFI_LIBC_ARCH 32
#elif defined(__x86_64__) || defined(__aarch64__) || defined(__mips64__) || defined(__riscv64__) || defined(__loongarch64)
#define UEFI_LIBC_ARCH 64
#elif 0 // 128-bit arch
#define UEFI_LIBC_ARCH 128
#else
#error "Unknown architecture define it manually by UEFI_LIBC_ARCH 32/64/128"
#endif
#endif

#endif //ARCH_H
