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
