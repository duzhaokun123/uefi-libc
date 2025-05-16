//
// Created by o0kam1 on 2025/05/13.
//

#include "FILE_Out.h"
#include <cstdio>

FILE_Out::FILE_Out(EFI_SIMPLE_TEXT_OUT_PROTOCOL* out) {
    _out = out;
}

int FILE_Out::fclose() {
    return EOF; // EFI_SIMPLE_TEXT_OUT_PROTOCOL not closable
}

int FILE_Out::fflush() {
    return 0; // no buffer for output
}

int FILE_Out::fgetc() {
    _error = EIO;
    return EOF; // no buffer readback for output
}

int FILE_Out::fputc(int c) {
    return fputwc(c); // FIXME: upcast char to wchar_t is NOT safe, it's only safe for ASCII to UTF-16
}

size_t FILE_Out::fread(void* ptr, size_t size, size_t count) {
    _error = EIO;
    return 0; // no buffer readback for output
}

size_t FILE_Out::fwrite(const void* ptr, size_t size, size_t count) {
    // EFI_SIMPLE_TEXT_OUT_PROTOCOL::OutputString(void* this, CHAR16* string) needs a null-terminated string
    // write bin data can cause out of bounds access
    _error = EIO;
    return 0;
}

int FILE_Out::fseek(long offset, int whence) {
    _error = ESPIPE;
    return -1; // no seek for output
}

long int FILE_Out::ftell() {
    return 0;
}

int FILE_Out::feof() {
    return 0; // EFI_SIMPLE_TEXT_OUT_PROTOCOL never reaches EOF
}

int FILE_Out::ferror() {
    return _error;
}

void FILE_Out::clearerr() {
    _error = 0;
}

wint_t FILE_Out::fgetwc() {
    _error = EIO;
    return WEOF; // no buffer readback for output
}

wint_t FILE_Out::fputwc(wchar_t c) {
    // XXX: do we need convert ANSI escape code (like "\033[0m") to set output?
    if (c == L'\n') {
        fputwc(L'\r'); // LF to CRLF
    }
    CHAR16 str[] = {static_cast<CHAR16>(c), '\0'};
    auto status = _out->OutputString(_out, static_cast<CHAR16*>(str));
    if (status != EFI_SUCCESS) {
        _error = EIO;
        return WEOF;
    }
    return c;
}
