//
// Created by o0kam1 on 2025/05/12.
//

#include <utils.h>
#include <cstring>

char* makeDosPath(const char* path) {
    if (path == nullptr) return nullptr;
    const auto len = strlen(path);
    const auto dosPath = new char[len + 1];
    for (size_t i = 0; i < len; i++) {
        if (path[i] == '/') {
            dosPath[i] = '\\';
        } else {
            dosPath[i] = path[i];
        }
    }
    dosPath[len] = '\0';
    return dosPath;
}

