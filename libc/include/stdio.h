//
// Created by o0kam1 on 2025/05/10.
//

#ifndef STDIO_H
#define STDIO_H

#include <restrict.h>

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char* restrict format, ...);
int putchar(int c);

#ifdef __cplusplus
}
#endif

#endif //STDIO_H
