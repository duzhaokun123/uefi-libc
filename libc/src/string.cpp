//
// Created by o0kam1 on 2025/05/10.
//

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include "algorithm"

void* memcpy(void* dst, const void* src, size_t n) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/string.c#L33
    uint8_t* a = (uint8_t*) dst,* b = (uint8_t*) src;
    if (src && dst && src != dst && n > 0) {
        while (n--) *a++ = *b++;
    }
    return dst;
}

void* memccpy(void* __restrict dest, const void* __restrict src, int c,
              size_t count) {
    // from llvm-project/libc/src/string/memccpy.cpp
    // https://github.com/llvm/llvm-project/blob/main/libc/src/string/memccpy.cpp
    unsigned char end = static_cast<unsigned char>(c);
    const unsigned char* uc_src = static_cast<const unsigned char*>(src);
    unsigned char* uc_dest = static_cast<unsigned char*>(dest);
    size_t i = 0;
    // Copy up until end is found.
    for (; i < count && uc_src[i] != end; ++i)
        uc_dest[i] = uc_src[i];
    // if i < count, then end must have been found, so copy end into dest and
    // return the byte after.
    if (i < count) {
        uc_dest[i] = uc_src[i];
        return uc_dest + i + 1;
    }
    return nullptr;
}

void* memmove(void* s1, const void* s2, size_t n) {
    if (static_cast<uint8_t*>(s1) + n <= s2 && static_cast<const uint8_t*>(s2) + n <= s1) {
        // No overlap, use memcpy
        return memcpy(s1, s2, n);
    }
    void* tmp = malloc(n);
    memcpy(tmp, s2, n);
    memcpy(s1, tmp, n);
    free(tmp);
    return s1;
}

char* strcpy(char* restrict s1, const char* restrict s2) {
    memcpy(s1, s2, strlen(s2) + 1);
    return s1;
}

char* strncpy(char* restrict s1, const char* restrict s2, size_t n) {
    auto srcLen = strlen(s2);
    srcLen = std::min(srcLen, n);
    memcpy(s1, s2, srcLen + 1);
    return s1;
}

char* strdup(const char* s) {
    const auto len = strlen(s);
    const auto r = static_cast<char*>(malloc(len + 1));
    memcpy(r, s, len + 1);
    return r;
}

char* strndup(const char* s, size_t n) {
    auto len = strlen(s);
    len = std::min(len, n);
    const auto r = static_cast<char*>(malloc(len + 1));
    memcpy(r, s, len);
    r[len] = 0;
    return r;
}

char* strcat(char* restrict s1, const char* restrict s2) {
    auto destLen = strlen(s1);
    auto srcLen = strlen(s2);
    memcpy(s1 + destLen, s2, srcLen + 1);
    return s1;
}

char* strncat(char* restrict s1, const char* restrict s2, size_t n) {
    auto destLen = strlen(s1);
    auto srcLen = strlen(s2);
    auto count = std::min(srcLen, n);
    memcpy(s1 + destLen, s2, count);
    s1[destLen + count] = 0;
    return s1;
}

int memcmp(const void* s1, const void* s2, size_t n) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/string.c#L64
    uint8_t* a = (uint8_t*) s1,* b = (uint8_t*) s2;
    if (s1 && s2 && s1 != s2 && n > 0) {
        while (n--) {
            if (*a != *b) return *a - *b;
            a++;
            b++;
        }
    }
    return 0;
}

int strcmp(const char* s1, const char* s2) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/string.c#L148
    if (s1 && s2 && s1 != s2) {
        while (*s1 && *s1 == *s2) {
            s1++;
            s2++;
        }
        return *s1 - *s2;
    }
    return 0;
}

int strcoll(const char* s1, const char* s2) {
    return strcmp(s1, s2); // XXX: check locale?
}

int strncmp(const char* s1, const char* s2, size_t n) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/string.c#L168
    const char* e = s1 + n - 1;
    if (s1 && s2 && s1 != s2 && n > 0) {
        while (s1 < e && *s1 && *s1 == *s2) {
            s1++;
            s2++;
        }
        return *s1 - *s2;
    }
    return 0;
}

size_t strxfrm(char* restrict s1, const char* restrict s2, size_t n);

void* memchr(const void* s, int c, size_t n) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/string.c#L76
    uint8_t* e,* p = (uint8_t*) s;
    if (s && n > 0) {
        for (e = p + n; p < e; p++) if (*p == (uint8_t) c) return p;
    }
    return NULL;
}

char* strchr(const char* s, int c) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/string.c#L186
    if (s) {
        while (*s) {
            if (*s == (char) c) return (char*) s;
            s++;
        }
    }
    return NULL;
}

size_t strcspn(const char* s1, const char* s2);

char* strpbrk(const char* s1, const char* s2);

char* strrchr(const char* s, int c) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/string.c#L197
    char* e;
    if (s) {
        e = (char*) s + strlen(s) - 1;
        while (s <= e) {
            if (*e == (char) c) return e;
            e--;
        }
    }
    return NULL;
}

size_t strspn(const char* s1, const char* s2);

char* strstr(const char* haystack, const char* needle) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/string.c#L210
    return (char*) memmem(haystack, strlen(haystack) * sizeof(char), needle, strlen(needle) * sizeof(char));
}

char* strtok(char* restrict s1, const char* restrict s2); // TODO: implement

void* memset(void* s, int c, size_t n) {
    for (size_t i = 0; i < n; i++) {
        static_cast<uint8_t*>(s)[i] = static_cast<uint8_t>(c);
    }
    return s;
}

void* memset_explicit(void* s, int c, size_t n) {
    return memset(s, c, n);
}

char strerror_buffer[256];

const char* errorMap[] = {
    "No error",
    "Operation not permitted",
    "No such file or directory",
    "No such process",
    "Interrupted system call",
    "Input/output error",
    "No such device or address",
    "Argument list too long",
    "Exec format error",
    "Bad file number",
    "No child processes",
    "Resource temporarily unavailable",
    "Cannot allocate memory",
    "Permission denied",
    "Bad address",
    "Block device required",
    "Device or resource busy",
    "File exists",
    "Cross-device link",
    "No such device",
    "Not a directory",
    "Is a directory",
    "Invalid argument",
    "File table overflow",
    "Too many open files",
    "Not a typewriter",
    "Text file busy",
    "File too large",
    "No space left on device",
    "Illegal seek",
    "Read-only file system",
    "Too many links",
    "Broken pipe",
    "Math argument out of domain of func",
    "Math result not representable",
};

char* strerror(int errnum) {
    const char* error = nullptr;
    if (errno >= 0 && errno <= ERANGE) {
        error = errorMap[errnum];
    }
    if (error == nullptr) {
        sprintf(strerror_buffer, "Unknown error (%d)", errnum);
    } else {
        strcpy(strerror_buffer, error);
    }
    return strerror_buffer;
}

size_t strlen(const char* s) {
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

size_t strnlen(const char* s, size_t n) {
    size_t len = 0;
    while (s[len] != '\0' && len <= n) {
        len++;
    }
    return len;
}

void* memmem(const void* haystack, size_t hl, const void* needle, size_t nl) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/string.c#L94
    uint8_t* c = (uint8_t*) haystack;
    if (!haystack || !needle || !hl || !nl || nl > hl) return NULL;
    hl -= nl - 1;
    while (hl) {
        if (!memcmp(c, needle, nl)) return c;
        c++;
        hl--;
    }
    return NULL;
}
