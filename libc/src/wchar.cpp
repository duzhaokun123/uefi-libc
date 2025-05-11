//
// Created by o0kam1 on 2025/05/10.
//

#include <cstdlib>
#include <cerrno>
#include <cwchar>

#include "TODO.h"

int fputws(const wchar_t* restrict s, FILE* restrict stream) {
    int len = 0;
    const wchar_t* p = s;
    while (*p) {
        const auto r = fputwc(*p, stream);
        if (r == WEOF) return WEOF;
        len++;
        p++;
    }
    return len;
}

wint_t text_out_putwc(wchar_t c, EFI_SIMPLE_TEXT_OUT_PROTOCOL* out) {
    if (c == '\n') {
        text_out_putwc('\r', out);
    }
    CHAR16 chr[] = {static_cast<CHAR16>(c), L'\0'};
    auto status = out->OutputString(out, chr);
    if (status != EFI_SUCCESS) {
        // TODO: handle error
    }
    return c;
}

wint_t file_putwc(wchar_t c, EFI_FILE_HANDLE file) {
    // TODO:
    TODO("Implement file_putwc");
}

wint_t fputwc(wchar_t c, FILE* stream) {
    errno = 0;
    switch (stream->type) {
        case FILE_STREAM_TYPE_IN:
        case FILE_STREAM_TYPE_IN_EX: {
            errno = EIO;
            return WEOF;
        }
        case FILE_STREAM_TYPE_OUT: {
            return text_out_putwc(c, stream->stream.text_out);
        }
        case FILE_STREAM_TYPE_FILE: {
            return file_putwc(c, stream->stream.file);
        }
            default: {
            errno = EINVAL;
            return WEOF;
        }
    }
}

wint_t putwc(wchar_t c, FILE* stream) {
    return fputwc(c, stream);
}

wint_t putwchar(wchar_t c) {
    return putwc(c, stdout);
}

int putws(const wchar_t* s) {
    auto r = fputws(s, stdout);
    if (r == WEOF) {
        return WEOF;
    }
    fputwc(L'\n', stdout);
    return r + 1;
}


