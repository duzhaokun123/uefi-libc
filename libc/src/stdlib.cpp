//
// Created by o0kam1 on 2025/05/10.
//

#include <uefi.h>
#include <cstdlib>

void* malloc(size_t size) {
    void* ptr = nullptr;
    EFI_STATUS status = BS->AllocatePool(LIP ? LIP->ImageDataType : EfiLoaderData, size, &ptr);
    if (status != EFI_SUCCESS) {
        return nullptr;
    }
    return ptr;
}

void free(void* ptr) {
    if (ptr == nullptr) return;
    BS->FreePool(ptr);
}

int wctomb(char* s, wchar_t u) {
    int ret = 0;
    if (u < 0x80) {
        *s = u;
        ret = 1;
    } else if (u < 0x800) {
        *(s + 0) = ((u >> 6) & 0x1F) | 0xC0;
        *(s + 1) = (u & 0x3F) | 0x80;
        ret = 2;
    } else {
        *(s + 0) = ((u >> 12) & 0x0F) | 0xE0;
        *(s + 1) = ((u >> 6) & 0x3F) | 0x80;
        *(s + 2) = (u & 0x3F) | 0x80;
        ret = 3;
    }
    return ret;
}

size_t wcstombs(char* __s, const wchar_t* __pwcs, size_t __n) {
    int r;
    char* orig = __s;
    if (!__s || !__pwcs || !*__pwcs) return 0;
    while (*__pwcs && ((size_t) (__s - orig + 4) < __n)) {
        r = wctomb(__s, *__pwcs);
        if (r < 0) return (size_t) -1;
        __pwcs++;
        __s += r;
    }
    *__s = 0;
    return (size_t) (__s - orig);
}
