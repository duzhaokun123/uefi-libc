//
// Created by o0kam1 on 2025/05/12.
//

#ifndef STDINT_H
#define STDINT_H

#define __STDC_VERSION_STDINT_H__ 202311L

#if defined(UEFI_LIBC_USE_GNU_EFI) && defined(_MSC_EXTENSIONS)
typedef char           int8_t; // gnu-efi use char as int8_t on detected _MSC_EXTENSIONS, make it happy
#else
typedef __INT8_TYPE__  int8_t;
#endif
typedef __INT16_TYPE__ int16_t;
typedef __INT32_TYPE__ int32_t;
typedef __INT64_TYPE__ int64_t;

typedef __INT_FAST8_TYPE__ int_fast8_t;
typedef __INT_FAST16_TYPE__ int_fast16_t;
typedef __INT_FAST32_TYPE__ int_fast32_t;
typedef __INT_FAST64_TYPE__ int_fast64_t;

typedef __INT_LEAST8_TYPE__ int_least8_t;
typedef __INT_LEAST16_TYPE__ int_least16_t;
typedef __INT_LEAST32_TYPE__ int_least32_t;
typedef __INT_LEAST64_TYPE__ int_least64_t;

typedef __INTMAX_TYPE__ intmax_t;
typedef __INTPTR_TYPE__ intptr_t;

typedef __UINT8_TYPE__  uint8_t;
typedef __UINT16_TYPE__ uint16_t;
typedef __UINT32_TYPE__ uint32_t;
#if defined(UEFI_LIBC_USE_GNU_EFI) && defined(__GNUC__)
typedef unsigned int __attribute__((__mode__(__DI__)))  uint64_t;
#else
typedef __UINT64_TYPE__ uint64_t;
#endif

typedef __UINT_FAST8_TYPE__ uint_fast8_t;
typedef __UINT_FAST16_TYPE__ uint_fast16_t;
typedef __UINT_FAST32_TYPE__ uint_fast32_t;
typedef __UINT_FAST64_TYPE__ uint_fast64_t;

typedef __UINT_LEAST8_TYPE__ uint_least8_t;
typedef __UINT_LEAST16_TYPE__ uint_least16_t;
typedef __UINT_LEAST32_TYPE__ uint_least32_t;
typedef __UINT_LEAST64_TYPE__ uint_least64_t;

typedef __UINTMAX_TYPE__ uintmax_t;
typedef __UINTPTR_TYPE__ uintptr_t;

// non-standard
#if __UINTPTR_MAX__ == __UINT32_MAX__ // 32-bit
typedef int32_t intn_t;
typedef uint32_t uintn_t;
#elif __UINTPTR_MAX__ == __UINT64_MAX__ // 64-bit
typedef int64_t intn_t;
typedef uint64_t uintn_t;
#else
#error "Unknown pointer size"
#endif

static_assert(sizeof(int8_t) == 1, "sizeof(int8_t) != 1");
static_assert(sizeof(int16_t) == 2, "sizeof(int16_t) != 2");
static_assert(sizeof(int32_t) == 4, "sizeof(int32_t) != 4");
static_assert(sizeof(int64_t) == 8, "sizeof(int64_t) != 8");
static_assert(sizeof(uint8_t) == 1, "sizeof(uint8_t) != 1");
static_assert(sizeof(uint16_t) == 2, "sizeof(uint16_t) != 2");
static_assert(sizeof(uint32_t) == 4, "sizeof(uint32_t) != 4");
static_assert(sizeof(uint64_t) == 8, "sizeof(uint64_t) != 8");


#endif //STDINT_H
