//
// Created by o0kam1 on 2025/05/10.
//

#ifndef STDLIB_H
#define STDLIB_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void* malloc(size_t size);
void free(void* ptr);
int wctomb(char* s, wchar_t wc);
size_t wcstombs(char* str, const wchar_t* pwcs, size_t n);

#ifdef __cplusplus
}
#endif

#endif //STDLIB_H
