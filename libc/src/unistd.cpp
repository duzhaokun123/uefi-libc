//
// Created by o0kam1 on 2025/05/12.
//

#include <cerrno>
#include <unistd.h>

int unlink(const char* path) {
    // TODO: Implement unlink
    errno = EIO;
    return -1;
}

int isatty(int fd) {
    errno = ENOTTY;
    return 0; // EFI_SIMPLE_TEXT_OUT may work link a terminal, but we have no way to check
}

