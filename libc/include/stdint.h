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

#define INT8_WIDTH 8
#define INT16_WIDTH 16
#define INT32_WIDTH 32
#define INT64_WIDTH 64

#define INT_FAST8_WIDTH __INT_FAST8_WIDTH__
#define INT_FAST16_WIDTH __INT_FAST16_WIDTH__
#define INT_FAST32_WIDTH __INT_FAST32_WIDTH__
#define INT_FAST64_WIDTH __INT_FAST64_WIDTH__

#define INT_LEAST8_WIDTH __INT_LEAST8_WIDTH__
#define INT_LEAST16_WIDTH __INT_LEAST16_WIDTH__
#define INT_LEAST32_WIDTH __INT_LEAST32_WIDTH__
#define INT_LEAST64_WIDTH __INT_LEAST64_WIDTH__

#define INTMAX_WIDTH __INTMAX_WIDTH__
#define INTPTR_WIDTH __INTPTR_WIDTH__

#define INT8_MIN (- __INT8_MAX__ - 1)
#define INT16_MIN (- __INT16_MAX__ - 1)
#define INT32_MIN (- __INT32_MAX__ - 1)
#define INT64_MIN (- __INT64_MAX__ - 1)

#define INT_FAST8_MIN (- __INT_FAST8_MAX__ - 1) // FIXME: this(s) fast/least min seems not right
#define INT_FAST16_MIN (- __INT_FAST16_MAX__ - 1)
#define INT_FAST32_MIN (- __INT_FAST32_MAX__ - 1)
#define INT_FAST64_MIN (- __INT_FAST64_MAX__ - 1)

#define INT_LEAST8_MIN (- __INT_LEAST8_MAX__ - 1)
#define INT_LEAST16_MIN (- __INT_LEAST16_MAX__ - 1)
#define INT_LEAST32_MIN (- __INT_LEAST32_MAX__ - 1)
#define INT_LEAST64_MIN (- __INT_LEAST64_MAX__ - 1)

#define INTMAX_MIN (- __INTMAX_MAX__ - 1)
#define INTPTR_MIN (- __INTPTR_MAX__ - 1)

#define INT8_MAX __INT8_MAX__
#define INT16_MAX __INT16_MAX__
#define INT32_MAX __INT32_MAX__
#define INT64_MAX __INT64_MAX__

#define INT_FAST8_MAX __INT_FAST8_MAX__
#define INT_FAST16_MAX __INT_FAST16_MAX__
#define INT_FAST32_MAX __INT_FAST32_MAX__
#define INT_FAST64_MAX __INT_FAST64_MAX__

#define INT_LEAST8_MAX __INT_LEAST8_MAX__
#define INT_LEAST16_MAX __INT_LEAST16_MAX__
#define INT_LEAST32_MAX __INT_LEAST32_MAX__
#define INT_LEAST64_MAX __INT_LEAST64_MAX__

#define INTMAX_MAX __INTMAX_MAX__
#define INTPTR_MAX __INTPTR_MAX__

#define UINT8_WIDTH 8
#define UINT16_WIDTH 16
#define UINT32_WIDTH 32
#define UINT64_WIDTH 64

// FIXME: define uint fast/least n width

#define UINTPTR_WIDTH __UINTPTR_WIDTH__
#define UINTMAX_WIDTH __UINTMAX_WIDTH__

#define UINT8_MAX __UINT8_MAX__
#define UINT16_MAX __UINT16_MAX__
#define UINT32_MAX __UINT32_MAX__
#define UINT64_MAX __UINT64_MAX__

#define UINT_FAST8_MAX __UINT_FAST8_MAX__
#define UINT_FAST16_MAX __UINT_FAST16_MAX__
#define UINT_FAST32_MAX __UINT_FAST32_MAX__
#define UINT_FAST64_MAX __UINT_FAST64_MAX__

#define UINT_LEAST8_MAX __UINT_LEAST8_MAX__
#define UINT_LEAST16_MAX __UINT_LEAST16_MAX__
#define UINT_LEAST32_MAX __UINT_LEAST32_MAX__
#define UINT_LEAST64_MAX __UINT_LEAST64_MAX__

#define UINTPTR_MAX __UINTPTR_MAX__
#define UINTMAX_MAX __UINTMAX_MAX__

#define INT8_C(value) value##__INT8_C_SUFFIX__
#define INT16_C(value) value##__INT16_C_SUFFIX__
#define INT32_C(value) value##__INT32_C_SUFFIX__
#define INT64_C(value) value##__INT64_C_SUFFIX__
#define INTMAX_C(value) value##__INTMAX_C_SUFFIX__

#define UINT8_C(value) value##__UINT8_C_SUFFIX__
#define UINT16_C(value) value##__UINT16_C_SUFFIX__
#define UINT32_C(value) value##__UINT32_C_SUFFIX__
#define UINT64_C(value) value##__UINT64_C_SUFFIX__
#define UINTMAX_C(value) value##__UINTMAX_C_SUFFIX__

static_assert(sizeof(int8_t) == 1, "sizeof(int8_t) != 1");
static_assert(sizeof(int16_t) == 2, "sizeof(int16_t) != 2");
static_assert(sizeof(int32_t) == 4, "sizeof(int32_t) != 4");
static_assert(sizeof(int64_t) == 8, "sizeof(int64_t) != 8");
static_assert(sizeof(uint8_t) == 1, "sizeof(uint8_t) != 1");
static_assert(sizeof(uint16_t) == 2, "sizeof(uint16_t) != 2");
static_assert(sizeof(uint32_t) == 4, "sizeof(uint32_t) != 4");
static_assert(sizeof(uint64_t) == 8, "sizeof(uint64_t) != 8");

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

#endif //STDINT_H
