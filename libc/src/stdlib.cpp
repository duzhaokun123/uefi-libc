//
// Created by o0kam1 on 2025/05/10.
//

#include <uefi.h>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include "algorithm"

typedef struct {
    size_t size;
    uint8_t data[];
} malloc_t;

void* malloc(size_t size) {
    malloc_t* ptr = nullptr;
    auto status = BS->AllocatePool(LIP ? LIP->ImageDataType : EfiLoaderData, size + offsetof(malloc_t, data), reinterpret_cast<void**>(&ptr));
    if (status != EFI_SUCCESS) {
        errno = ENOMEM;
        return nullptr;
    }
    ptr->size = size;
    return reinterpret_cast<uint8_t*>(ptr) + offsetof(malloc_t, data);
}

void free(void* ptr) {
    if (ptr == nullptr) return;
    auto mallocPtr = reinterpret_cast<malloc_t*>(static_cast<uint8_t*>(ptr) - offsetof(malloc_t, data));
    BS->FreePool(mallocPtr);
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

char* getenv(const char* name) {
    // FIXME: why assert it's utf-16 wchar array
    size_t size = 0;
    auto ret = getenvb(name, &size);
    if (size == 0) {
        return nullptr;
    }
    auto str = static_cast<char*>(malloc(size + 1));
    wcstombs(str, reinterpret_cast<wchar_t*>(ret), size);
    str[size] = 0;
    free(ret);
    return str;
}

void* realloc(void* ptr, size_t size) {
    if (ptr == nullptr) return malloc(size);
    auto newPtr = malloc(size);
    if (newPtr == nullptr) {
        return nullptr;
    }
    const auto oldMallocPtr = reinterpret_cast<malloc_t*>(static_cast<uint8_t*>(ptr) - offsetof(malloc_t, data));
    memcpy(newPtr, ptr, std::min(oldMallocPtr->size, size));
    free(ptr);
    return newPtr;
}

int mbtowc(wchar_t* __pwc, const char* s, size_t n) {
    wchar_t arg;
    int ret = 1;
    if (!s || !*s) return 0;
    arg = (wchar_t) *s;
    if ((*s & 128) != 0) {
        if ((*s & 32) == 0 && n > 0) {
            arg = ((*s & 0x1F) << 6) | (*(s + 1) & 0x3F);
            ret = 2;
        } else if ((*s & 16) == 0 && n > 1) {
            arg = ((*s & 0xF) << 12) | ((*(s + 1) & 0x3F) << 6) | (*(s + 2) & 0x3F);
            ret = 3;
        } else if ((*s & 8) == 0 && n > 2) {
            arg = ((*s & 0x7) << 18) | ((*(s + 1) & 0x3F) << 12) | ((*(s + 2) & 0x3F) << 6) | (*(s + 3) & 0x3F);
            ret = 4;
        } else return -1;
    }
    if (__pwc) *__pwc = arg;
    return ret;
}

size_t mbstowcs(wchar_t* __pwcs, const char* __s, size_t __n) {
    int r;
    wchar_t* orig = __pwcs;
    if (!__s || !*__s) return 0;
    while (*__s) {
        r = mbtowc(__pwcs, __s, __n - (size_t) (__pwcs - orig));
        if (r < 0) return (size_t) -1;
        __pwcs++;
        __s += r;
    }
    *__pwcs = 0;
    return (size_t) (__pwcs - orig);
}

[[noreturn]]
void abort() {
    _cleanup();
    BS->Exit(IM, EFI_ABORTED, 0, nullptr);
    while (true) {}
}

uint8_t* getenvb(const char* name, size_t* size) {
    EFI_GUID globalGuid = EFI_GLOBAL_VARIABLE;
    uint8_t tmp[EFI_MAXIMUM_VARIABLE_SIZE];
    uint32_t attr;
    wchar_t wname[256];
    mbstowcs(reinterpret_cast<wchar_t*>(&wname), name, 256);
    auto status = RT->GetVariable(reinterpret_cast<CHAR16*>(&wname), &globalGuid, &attr, reinterpret_cast<uintn_t*>(size), tmp);
    if (status != EFI_SUCCESS || *size < 1) {
        *size = 0;
        return nullptr;
    }
    auto ret = static_cast<uint8_t*>(malloc(*size));
    memcpy(ret, tmp, *size);
    ret[*size] = 0;
    return ret;
}

void* calloc(size_t nmemb, size_t size) {
    return malloc(nmemb * size);
}

int abs(int j) {
    return j < 0 ? -j : j;
}

[[noreturn]]
void exit(int status) {
    _cleanup();
    EFI_STATUS r = status != 0 ? EFIERR(abs(status)) : EFI_SUCCESS;
    BS->Exit(IM, r, 0, nullptr);
    while (true) {}
}
