//
// Created by o0kam1 on 2025/05/10.
//

#ifndef ASSERT_H
#define ASSERT_H

#define __STDC_VERSION_ASSERT_H__ 202311L

#include <stdlib.h>

#ifdef NDEBUG
#define assert(e) ((void)0)
#else
#define assert(e) \
    do { \
        if (!(e)) { \
            printf("Assertion failed: %s, file %s, line %d\n", #e, __FILE__, __LINE__); \
            abort(); \
        } \
    } while (0)
#endif

#endif //ASSERT_H
