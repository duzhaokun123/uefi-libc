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
        return EOF; // TODO: set ferror
    }
    // XXX: do we need convert EFI_INPUT_KEY::ScanCode to ANSI escape code?
    // FIXME: convert UTF-16 wchar to multi-byte char
    return key.UnicodeChar;
}

int FILE_In::fputc(int c) {
    return EOF; // not writable, TODO: set ferror
}

size_t FILE_In::fread(void* ptr, size_t size, size_t count) {
    // XXX: do we need convert EFI_INPUT_KEY to UTF-16 string?
    return 0; // TODO: set ferror
}

size_t FILE_In::fwrite(const void* ptr, size_t size, size_t count) {
    return 0; // not writable, TODO: set ferror
}

int FILE_In::fseek(long offset, int whence) {
    return -1; // not seekable, TODO: set ferror
}

long int FILE_In::ftell() {
    return 0;
}

int FILE_In::feof() {
    return 0; // EFI_SIMPLE_TEXT_IN_PROTOCOL never reaches EOF
}

int FILE_In::ferror() {
    return 0; // TODO: get error status
}

void FILE_In::clearerr() {
    // TODO: clear error status
}

wint_t FILE_In::fgetwc() {
    uintn_t eventIndex = 0;
    BS->WaitForEvent(1, &_in->WaitForKey, &eventIndex);
    EFI_INPUT_KEY key;
    auto status = _in->ReadKeyStroke(_in, &key);
    if (status != EFI_SUCCESS) {
        return EOF; // TODO: set ferror
    }
    // XXX: do we need convert EFI_INPUT_KEY::ScanCode to ANSI escape code?
    return key.UnicodeChar;
}

wint_t FILE_In::fputwc(wchar_t c) {
    return WEOF; // not writable, TODO: set ferror
}

