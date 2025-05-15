//
// Created by o0kam1 on 2025/05/10.
//

#include <cstdlib>
#include <cerrno>
#include <cwchar>

#include "utils/TODO.h"
#include "stdio/FILE_stream.h"
#include "utils/STUB.h"

int fwprintf(FILE* restrict stream, const wchar_t* restrict format, ...) {
    va_list args;
    va_start(args, format);
    auto r = vfwprintf(stream, format, args);
    va_end(args);
    return r;
}

int fwscanf(FILE* restrict stream, const wchar_t* restrict format, ...) {
    va_list args;
    va_start(args, format);
    auto r = vfwscanf(stream, format, args);
    va_end(args);
    return r;
}

int swprintf(wchar_t* restrict s, size_t n, const wchar_t* restrict format, ...) {
    va_list args;
    va_start(args, format);
    auto r = vswprintf(s, n, format, args);
    va_end(args);
    return r;
}

int swscanf(const wchar_t* restrict s, const wchar_t* restrict format, ...) {
    va_list args;
    va_start(args, format);
    auto r = vswscanf(s, format, args);
    va_end(args);
    return r;
}

int vfwprintf(FILE* restrict stream, const wchar_t* restrict format, va_list arg) {
    TODO("implement vfwprintf");
}

int vfwscanf(FILE* restrict stream, const wchar_t* restrict format, va_list arg) {
    TODO("implement vfwscanf");
}

int vswprintf(wchar_t* restrict s, size_t n, const wchar_t* restrict format, va_list arg) {
    TODO("implement vswprintf");
}

int vswscanf(const wchar_t* restrict s, const wchar_t* restrict format, va_list arg) {
    TODO("implement vswscanf");
}

int vwprintf(const wchar_t* restrict format, va_list arg) {
    return vfwprintf(stdout, format, arg);
}

int vwscanf(const wchar_t* restrict format, va_list arg) {
    return vfwscanf(stdin, format, arg);
}

int wprintf(const wchar_t* restrict format, ...) {
    va_list args;
    va_start(args, format);
    auto r = vwprintf(format, args);
    va_end(args);
    return r;
}

int wscanf(const wchar_t* restrict format, ...) {
    va_list args;
    va_start(args, format);
    auto r = vwscanf(format, args);
    va_end(args);
    return r;
}

wint_t fgetwc(FILE* stream) {
    return reinterpret_cast<FILE_stream*>(stream)->fgetwc();
}

wchar_t* fgetws(wchar_t* restrict s, int n, FILE* restrict stream) {
    int len = 0;
    while (len < n - 1) {
        auto c = fgetwc(stream);
        if (c == WEOF) {
            break;
        }
        if (c == L'\n') {
            break;
        }
        s[len] = c;
        len++;
    }
    s[len] = L'\0';
    if (ferror(stream)) {
        return nullptr;
    }
    return len == 0 ? nullptr : s;
}

wint_t fputwc(wchar_t c, FILE* stream) {
    return reinterpret_cast<FILE_stream*>(stream)->fputwc(c);
}

int fputws(const wchar_t* restrict s, FILE* restrict stream) {
    int len = 0;
    while (s[len] != L'\0') {
        fputwc(s[len], stream);
        len++;
    }
    return len;
}

FUNC_STUB(
int fwide(FILE* stream, int mode)
)

wint_t getwc(FILE* stream) {
    return fgetwc(stream);
}

wint_t getwchar() {
    return fgetwc(stdin);
}

wint_t putwc(wchar_t c, FILE* stream) {
    return fputwc(c, stream);
}

wint_t putwchar(wchar_t c) {
    return fputwc(c, stdout);
}

FUNC_STUB(
wint_t ungetwc(wint_t c, FILE* stream)
)

// some other functions

int putws(const wchar_t* s) {
    return fputws(s, stdout);
}
