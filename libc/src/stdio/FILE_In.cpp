//
// Created by o0kam1 on 2025/05/13.
//

#include "FILE_In.h"

#include <cstdint>
#include <cstdio>

#include "uefi.h"

FILE_In::FILE_In(EFI_SIMPLE_TEXT_IN_PROTOCOL* in) {
    _in = in;
}

int FILE_In::fclose() {
    return EOF; // EFI_SIMPLE_TEXT_IN_PROTOCOL not closable
}

int FILE_In::fflush() {
    return 0; // no buffer for input
}

int FILE_In::fgetc() {
    uintn_t eventIndex = 0;
    BS->WaitForEvent(1, &_in->WaitForKey, &eventIndex);
    EFI_INPUT_KEY key;
    auto status = _in->ReadKeyStroke(_in, &key);
    if (status != EFI_SUCCESS) {
        _error = EIO;
        return EOF;
    }
    // XXX: do we need convert EFI_INPUT_KEY::ScanCode to ANSI escape code?
    // FIXME: convert UTF-16 wchar to multi-byte char
    return key.UnicodeChar;
}

int FILE_In::fputc(int c) {
    _error = EIO;
    return EOF;
}

size_t FILE_In::fread(void* ptr, size_t size, size_t count) {
    // XXX: do we need convert EFI_INPUT_KEY to UTF-16 string?
    _error = EIO;
    return 0;
}

size_t FILE_In::fwrite(const void* ptr, size_t size, size_t count) {
    _error = EIO;
    return 0; // not writable
}

int FILE_In::fseek(long offset, int whence) {
    _error = ESPIPE;
    return -1; // not seekable
}

long int FILE_In::ftell() {
    return 0;
}

int FILE_In::feof() {
    return 0; // EFI_SIMPLE_TEXT_IN_PROTOCOL never reaches EOF
}

int FILE_In::ferror() {
    return _error;
}

void FILE_In::clearerr() {
    _error = 0;
}

wint_t FILE_In::fgetwc() {
    uintn_t eventIndex = 0;
    BS->WaitForEvent(1, &_in->WaitForKey, &eventIndex);
    EFI_INPUT_KEY key;
    auto status = _in->ReadKeyStroke(_in, &key);
    if (status != EFI_SUCCESS) {
        _error = EIO;
        return EOF;
    }
    // XXX: do we need convert EFI_INPUT_KEY::ScanCode to ANSI escape code?
    return key.UnicodeChar;
}

wint_t FILE_In::fputwc(wchar_t c) {
    _error = EIO;
    return WEOF;
}

