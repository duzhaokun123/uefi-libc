//
// Created by o0kam1 on 2025/05/13.
//

#include "FILE_Out.h"

#include <stdio.h>

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
    return EOF; // no buffer readback for output, TODO: set ferror
}

int FILE_Out::fputc(int c) {
    return fputwc(c); // upcast char to wchar_t is safe
}

size_t FILE_Out::fread(void* ptr, size_t size, size_t count) {
    return 0; // no buffer readback for output, TODO: set ferror
}

size_t FILE_Out::fwrite(const void* ptr, size_t size, size_t count) {
    // EFI_SIMPLE_TEXT_OUT_PROTOCOL::OutputString(void* this, CHAR16* string) needs a null-terminated string
    // write bin data can cause out of bounds access
    return 0; // TODO: set ferror
}

int FILE_Out::fseek(long offset, int whence) {
    return -1; // no seek for output
}

long int FILE_Out::ftell() {
    return 0;
}

int FILE_Out::feof() {
    return 0; // EFI_SIMPLE_TEXT_OUT_PROTOCOL never reaches EOF
}

int FILE_Out::ferror() {
    return 0; // TODO: get error status
}

void FILE_Out::clearerr() {
    // TODO: clear error status
}

wint_t FILE_Out::fgetwc() {
    return WEOF; // no buffer readback for output
}

wint_t FILE_Out::fputwc(wchar_t c) {
    // XXX: do we need convert ANSI escape code (like "\033[0m") to set output?
    if (c == L'\n') {
        fputwc(L'\r'); // LF to CRLF
    }
    CHAR16 str[] = {static_cast<CHAR16>(c), '\0'};
    _out->OutputString(_out, static_cast<CHAR16*>(str));
    return c; // TODO: check status
}
