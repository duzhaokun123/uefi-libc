//
// Created by o0kam1 on 2025/05/10.
//

#include <cstdio>
#include <uefi.h>

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#define NANOPRINTF_IMPLEMENTATION
#include "nanoprintf.h"

int putchar(int c) {
    if (c == '\n') {
        putchar('\r');
    }
    CHAR16 chr[] = {static_cast<CHAR16>(c), L'\0'};
    auto status = ST->ConOut->OutputString(ST->ConOut, chr);
    if (status != EFI_SUCCESS) {
        // TODO: handle error
    }
    return c;
}

int printf(const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    const auto r = npf_vpprintf([](int c, void*) { putchar(c); }, nullptr, format, args);
    va_end(args);
    return r;
}
