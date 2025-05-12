//
// Created by o0kam1 on 2025/05/12.
//

#ifndef STDINT_H
#define STDINT_H

#ifndef UEFI_CLANG_FAKE_WIN32
#include_next <stdint.h>
#else
typedef char                int8_t;
typedef short               int16_t;
typedef int                 int32_t;
typedef long long           int64_t;
typedef unsigned char       uint8_t;
typedef unsigned short      uint16_t;
typedef unsigned int        uint32_t;
typedef unsigned long long  uint64_t;
typedef unsigned long long  uintptr_t;
#define __int8_t_defined

#include_next <stdint.h>
#endif

#endif //STDINT_H
