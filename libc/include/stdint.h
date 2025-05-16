//
// Created by o0kam1 on 2025/05/12.
//

#ifndef STDINT_H
#define STDINT_H

#define __STDC_VERSION_STDINT_H__ 202311L

#include <arch.h>

#if defined(UEFI_LIBC_USE_GNU_EFI) && defined(__GNUC__)
typedef signed char     int8_t;
#else
typedef char            int8_t;
#endif
typedef short           int16_t;
typedef int             int32_t;
typedef long long int   int64_t;

typedef signed char     int_fast8_t;
typedef short           int_fast16_t;
typedef int             int_fast32_t;
typedef long long int   int_fast64_t;

typedef signed char     int_least8_t;
typedef short           int_least16_t;
typedef int             int_least32_t;
typedef long long int   int_least64_t;

typedef int64_t intmax_t;

typedef unsigned char           uint8_t;
typedef unsigned short          uint16_t;
typedef unsigned int            uint32_t;
typedef unsigned long long int  uint64_t;

typedef unsigned char           uint_fast8_t;
typedef unsigned short          uint_fast16_t;
typedef unsigned int            uint_fast32_t;
typedef unsigned long long int  uint_fast64_t;

typedef unsigned char           uint_least8_t;
typedef unsigned short          uint_least16_t;
typedef unsigned int            uint_least32_t;
typedef unsigned long long int  uint_least64_t;

typedef uint64_t uintmax_t;

#define INT8_WIDTH  8
#define INT16_WIDTH 16
#define INT32_WIDTH 32
#define INT64_WIDTH 64

#define INT_FAST8_WIDTH     8
#define INT_FAST16_WIDTH    16
#define INT_FAST32_WIDTH    32
#define INT_FAST64_WIDTH    64

#define INT_LEAST8_WIDTH    8
#define INT_LEAST16_WIDTH   16
#define INT_LEAST32_WIDTH   32
#define INT_LEAST64_WIDTH   64

#define INTMAX_WIDTH 64

#define INT8_MIN    (-128)
#define INT16_MIN   (-32767-1)
#define INT32_MIN   (-2147483647-1)
#define INT64_MIN   (-9223372036854775807L-1)

#define INT_FAST8_MIN   (-127)
#define INT_FAST16_MIN  (-32767-1)
#define INT_FAST32_MIN  (-2147483647-1)
#define INT_FAST64_MIN  (-9223372036854775807L-1)

#define INT_LEAST8_MIN  (-127)
#define INT_LEAST16_MIN (-32767-1)
#define INT_LEAST32_MIN (-2147483647-1)
#define INT_LEAST64_MIN (-9223372036854775807L-1)

#define INTMAX_MIN  (-9223372036854775807L-1)

#define INT8_MAX    127
#define INT16_MAX   32767
#define INT32_MAX   2147483647
#define INT64_MAX   9223372036854775807L

#define INT_FAST8_MAX   127
#define INT_FAST16_MAX  32767
#define INT_FAST32_MAX  2147483647
#define INT_FAST64_MAX  9223372036854775807L

#define INT_LEAST8_MAX  127
#define INT_LEAST16_MAX 32767
#define INT_LEAST32_MAX 2147483647
#define INT_LEAST64_MAX 9223372036854775807L

#define INTMAX_MAX  9223372036854775807L

#define UINT8_WIDTH     8
#define UINT16_WIDTH    16
#define UINT32_WIDTH    32
#define UINT64_WIDTH    64

#define UINT_FAST8_WIDTH    8
#define UINT_FAST16_WIDTH   16
#define UINT_FAST32_WIDTH   32
#define UINT_FAST64_WIDTH   64

#define UINT_LEAST8_WIDTH   8
#define UINT_LEAST16_WIDTH  16
#define UINT_LEAST32_WIDTH  32
#define UINT_LEAST64_WIDTH  64

#define UINTMAX_WIDTH   64

#define UINT8_MAX   255
#define UINT16_MAX  65535
#define UINT32_MAX  4294967295U
#define UINT64_MAX  18446744073709551615LU

#define UINT_FAST8_MAX  255
#define UINT_FAST16_MAX 65535
#define UINT_FAST32_MAX 4294967295U
#define UINT_FAST64_MAX 18446744073709551615LU

#define UINT_LEAST8_MAX     255
#define UINT_LEAST16_MAX    65535
#define UINT_LEAST32_MAX    4294967295U
#define UINT_LEAST64_MAX    18446744073709551615LU

#define UINTMAX_MAX     18446744073709551615LU

#define INT8_C(c)   c
#define INT16_C(c)  c
#define INT32_C(c)  c
#define INT64_C(c)  c##L
#define INTMAX_C(c) c##L

#define UINT8_C(c)      c##U
#define UINT16_C(c)     c##U
#define UINT32_C(c)     c##U
#define UINT64_C(c)     c##UL
#define UINTMAX_C(c)    c##UL

// non-standard ========================================================================================================
// https://uefi.org/specs/UEFI/2.11/02_Overview.html#data-types
// INT128, UINT128 16-byte value
typedef __int128 int128_t;
#define INT128_WIDTH 128
#define INT128_MIN (-170141183460469231731687303715884105728LL)
#define INT128_MAX  170141183460469231731687303715884105727LL
typedef unsigned __int128 uint128_t;
#define UINT128_WIDTH 128
#define UINT128_MAX  340282366920938463463374607431768211455LL

// end non-standard ====================================================================================================

#if UEFI_LIBC_ARCH == 32
typedef int32_t intptr_t;
#define INTPTR_WIDTH INT32_WIDTH
#define INTPTR_MIN INT32_MIN
#define INTPTR_MAX INT32_MAX
typedef uint32_t uintptr_t;
#define UINTPTR_WIDTH UINT32_WIDTH
#define UINTPTR_MAX UINT32_MAX
#define PTRDIFF_WIDTH INT32_WIDTH
#define PTRDIFF_MIN INT32_MIN
#define PTRDIFF_MAX INT32_MAX
#define SIZE_WIDTH INT32_WIDTH
#define SIZE_MAX INT32_MAX
#elif UEFI_LIBC_ARCH == 64
typedef int64_t intptr_t;
#define INTPTR_WIDTH INT64_WIDTH
#define INTPTR_MIN INT64_MIN
#define INTPTR_MAX INT64_MAX
typedef uint64_t uintptr_t;
#define UINTPTR_WIDTH UINT64_WIDTH
#define UINTPTR_MAX UINT64_MAX
#define PTRDIFF_WIDTH INT64_WIDTH
#define PTRDIFF_MIN INT64_MIN
#define PTRDIFF_MAX INT64_MAX
#define SIZE_WIDTH INT64_WIDTH
#define SIZE_MAX INT64_MAX
#elif UEFI_LIBC_ARCH == 128
typedef int128_t intptr_t;
#define INTPTR_WIDTH INT128_WIDTH
#define INTPTR_MIN INT128_MIN
#define INTPTR_MAX INT128_MAX
typedef uint128_t uintptr_t;
#define UINTPTR_WIDTH UINT128_WIDTH
#define UINTPTR_MAX UINT128_MAX
#define PTRDIFF_WIDTH INT128_WIDTH
#define PTRDIFF_MIN INT128_MIN
#define PTRDIFF_MAX INT128_MAX
#define SIZE_WIDTH INT128_WIDTH
#define SIZE_MAX INT128_MAX
#endif

// non-standard ========================================================================================================
// https://uefi.org/specs/UEFI/2.11/02_Overview.html#data-types
// INTN, UINTN native value
typedef intptr_t  intn_t;
#define INTN_WIDTH INTPTR_WIDTH
#define INTN_MIN INTPTR_MIN
#define INTN_MAX INTPTR_MAX
typedef uintptr_t uintn_t;
#define UINTN_WIDTH UINTPTR_WIDTH
#define UINTN_MAX UINTPTR_MAX
// end non-standard ====================================================================================================

// checks
static_assert(sizeof(int8_t) == 1, "sizeof(int8_t) != 1");
static_assert(sizeof(int16_t) == 2, "sizeof(int16_t) != 2");
static_assert(sizeof(int32_t) == 4, "sizeof(int32_t) != 4");
static_assert(sizeof(int64_t) == 8, "sizeof(int64_t) != 8");
#if UEFI_LIBC_ARCH == 32
static_assert(sizeof(intptr_t) == 4, "sizeof(intptr_t) != 4");
#elif UEFI_LIBC_ARCH == 64
static_assert(sizeof(intptr_t) == 8, "sizeof(intptr_t) != 8");
#elif UEFI_LIBC_ARCH == 128
static_assert(sizeof(intptr_t) == 16, "sizeof(intptr_t) != 16");
#endif

#endif //STDINT_H
