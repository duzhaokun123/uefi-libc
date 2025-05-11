//
// Created by o0kam1 on 2025/05/10.
//

#include <cstring>
#include <efi.h>

#include <cerrno>

#include "cstdio"

size_t strlen(const char* s) {
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

int strcmp(const char* s1, const char* s2) {
    if (s1 && s2 && s1 != s2) {
        while (*s1 && *s1 == *s2) {
            s1++;
            s2++;
        }
        return *s1 - *s2;
    }
    return 0;
}


int memcmp(const void* s1, const void* s2, size_t n) {
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

void* memcpy(void* dst, const void* src, size_t n) {
    uint8_t* a = (uint8_t*) dst,* b = (uint8_t*) src;
    if (src && dst && src != dst && n > 0) {
        while (n--) *a++ = *b++;
    }
    return dst;
}

int strncmp(const char* s1, const char* s2, size_t n) {
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

char* strchr(const char* s, int c) {
    if (s) {
        while (*s) {
            if (*s == (char) c) return (char*) s;
            s++;
        }
    }
    return NULL;
}

void* memset(void* s, int c, size_t n) {
    uint8_t* p = (uint8_t*) s;
    if (s && n > 0) {
        while (n--) *p++ = (uint8_t) c;
    }
    return s;
}

void* memchr(const void* s, int c, size_t n) {
    uint8_t* e,* p = (uint8_t*) s;
    if (s && n > 0) {
        for (e = p + n; p < e; p++) if (*p == (uint8_t) c) return p;
    }
    return NULL;
}

char* strstr(const char* haystack, const char* needle) {
    return (char*) memmem(haystack, strlen(haystack) * sizeof(char), needle, strlen(needle) * sizeof(char));
}

void* memmem(const void* haystack, size_t hl, const void* needle, size_t nl) {
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

char* strcpy(char* dst, const char* src) {
    char* s = dst;
    if (src && dst && src != dst) {
        while (*src) { *dst++ = *src++; }
        *dst = 0;
    }
    return s;
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
