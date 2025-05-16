//
// Created by o0kam1 on 2025/05/10.
//

#include <uefi.h>
#include <cstdlib>
#include <cerrno>
#include <cstring>

#include "algorithm"
#include "utils/STUB.h"

typedef struct {
    size_t size;
    uint8_t data[];
} malloc_t;

// void call_once(once_flag* flag, void (*func)(void));

FUNC_STUB(
double atof(const char* nptr)
);

int atoi(const char* nptr) {
    return atoll(nptr);
}

long int atol(const char* nptr) {
    return atoll(nptr);
}

long long int atoll(const char* nptr) {
    // FIXME: it looks bad
    long long r = 0;
    bool negative = false;
    for (size_t i = 0; nptr[i] != '\0'; i++) {
        char ch = nptr[i];
        if (ch == '-') {
            negative = true;
            continue;
        }
        if (ch == '+') {
            continue;
        }
        if (ch >= '0' && ch <= '9') {
            r = r * 10 + (ch - '0');
            continue;
        }
        if (ch == ' ') {
            continue;
        }
        return 0; // invalid character
    }
    return negative ? -r : r;
}

// some other functions are not implemented yet

int randNext = 0;

int rand() {
    auto r = randNext;
    randNext = (randNext * 1103515245 + 12345) & 0x7FFFFFFF; // FIXME: idk, github copilot made this
    return r;
}

void srand(unsigned int seed) {
    randNext = seed;
}

void* aligned_alloc(size_t alignment, size_t size);

void* calloc(size_t nmemb, size_t size) {
    auto ptr = malloc(nmemb * size);
    if (ptr == nullptr) {
        return nullptr;
    }
    memset(ptr, 0, nmemb * size);
    return ptr;
}

void free(void* ptr) {
    if (ptr == nullptr) return;
    auto mallocPtr = reinterpret_cast<malloc_t*>(static_cast<uint8_t*>(ptr) - offsetof(malloc_t, data));
    BS->FreePool(mallocPtr);
}

void free_sized(void* ptr, size_t size) {
    free(ptr);
}

FUNC_STUB(
void free_aligned_sized(void* ptr, size_t alignment, size_t size)
)

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

[[noreturn]]
void abort() {
    _cleanup();
    BS->Exit(IM, EFI_ABORTED, 0, nullptr);
    while (true) {} // unreachable
}

int atexit(void (* func)()) {
    return -1; // TODO: implement
}

int at_quick_exit(void (* func)()) {
    return -1; // TODO: implement
}

[[noreturn]]
void exit(int status) {
    // TODO: call atexit functions
    _cleanup();
    EFI_STATUS r = status != 0 ? EFIERR(abs(status)) : EFI_SUCCESS;
    BS->Exit(IM, r, 0, nullptr);
    while (true) {} // unreachable
}

[[noreturn]]
void _Exit(int status) {
    _cleanup();
    EFI_STATUS r = status != 0 ? EFIERR(abs(status)) : EFI_SUCCESS;
    BS->Exit(IM, r, 0, nullptr);
    while (true) {} // unreachable
}

char* getenv(const char* name) {
    // FIXME: why assert it's utf-16 wchar array
    size_t size = 0;
    auto ret = getenvb(name, &size);
    if (size == 0) {
        return nullptr;
    }
    auto str = static_cast<char*>(malloc(size * 2 + 1));
    wcstombs(str, reinterpret_cast<wchar_t*>(ret), size * 2 + 1);
    str[size] = 0;
    free(ret);
    return str;
}

[[noreturn]]
void quick_exit(int status) {
    // TODO: call at_quick_exit functions
    _cleanup();
    EFI_STATUS r = status != 0 ? EFIERR(abs(status)) : EFI_SUCCESS;
    BS->Exit(IM, r, 0, nullptr);
    while (true) {} // unreachable
}

int system(const char* string) {
    if (string == nullptr) {
        return 0; // TODO: check if EFI_SHELL_PROTOCOL exist
    } else {
        return 1; // TODO: use EFI_SHELL_PROTOCOL::Execute(EFI_HANDLE* caller, CHAR16* commandLine, CHAR16** environment, EFI_STATUS* exitStatus)
    }
}

void* bsearch(const void* key, const void* array, size_t array_size,
              size_t elem_size,
              int (* compare)(const void*, const void*)) {
    // from llvm-project/libc/src/stdlib/bsearch.cpp
    // https://github.com/llvm/llvm-project/blob/main/libc/src/stdlib/bsearch.cpp
    if (key == nullptr || array == nullptr || array_size == 0 || elem_size == 0)
        return nullptr;

    while (array_size > 0) {
        size_t mid = array_size / 2;
        const void* elem =
                reinterpret_cast<const uint8_t*>(array) + mid * elem_size;
        int compare_result = compare(key, elem);
        if (compare_result == 0)
            return const_cast<void*>(elem);

        if (compare_result < 0) {
            // This means that key is less than the element at |mid|.
            // So, in the next iteration, we only compare elements less
            // than mid.
            array_size = mid;
        } else {
            // |mid| is strictly less than |array_size|. So, the below
            // decrement in |array_size| will not lead to a wrap around.
            array_size -= (mid + 1);
            array = reinterpret_cast<const uint8_t*>(elem) + elem_size;
        }
    }

    return nullptr;
}

FUNC_STUB(
void qsort(void* base, size_t nmemb, size_t size,
           int (* compar)(const void*, const void*))
)

int abs(int j) {
    return j < 0 ? -j : j;
}

long int labs(long int j) {
    return j < 0 ? -j : j;
}

long long int llabs(long long int j) {
    return j < 0 ? -j : j;
}

div_t div(int numer, int denom) {
    // compiler will optimize this to use div instruction, right?
    div_t r;
    r.quot = numer / denom;
    r.rem = numer % denom;
    return r;
}

ldiv_t ldiv(long int numer, long int denom) {
    // compiler will optimize this to use div instruction, right?
    ldiv_t r;
    r.quot = numer / denom;
    r.rem = numer % denom;
    return r;
}

lldiv_t lldiv(long long int numer, long long int denom) {
    // compiler will optimize this to use div instruction, right?
    lldiv_t r;
    r.quot = numer / denom;
    r.rem = numer % denom;
    return r;
}

int mblen(const char* s, size_t n) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/stdlib.c#L228
    const char* e = s + n;
    int c = 0;
    if (s) {
        while (s < e && *s) {
            if ((*s & 128) != 0) {
                if ((*s & 32) == 0) s++;
                else if ((*s & 16) == 0) s += 2;
                else
                    if ((*s & 8) == 0) s += 3;
            }
            c++;
            s++;
        }
    }
    return c;
}

int mbtowc(wchar_t* __pwc, const char* s, size_t n) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/stdlib.c#L246
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

int wctomb(char* s, wchar_t u) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/stdlib.c#L262
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

size_t mbstowcs(wchar_t* __pwcs, const char* __s, size_t __n) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/stdlib.c#L281
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

size_t wcstombs(char* __s, const wchar_t* __pwcs, size_t __n) {
    // from posix-uefi
    // https://gitlab.com/bztsrc/posix-uefi/-/blob/master/uefi/stdlib.c#L296
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

FUNC_STUB(
size_t memalignment(const void* p)
)

uint8_t* getenvb(const char* name, size_t* size) {
    EFI_GUID globalGuid = EFI_GLOBAL_VARIABLE;
    wchar_t wname[256];
    mbstowcs(reinterpret_cast<wchar_t*>(&wname), name, 256);
    uintn_t bufferSize = 0;
    RT->GetVariable(reinterpret_cast<CHAR16*>(wname), &globalGuid, nullptr, &bufferSize, nullptr);
    auto buffer = static_cast<uint8_t*>(malloc(bufferSize));
    auto status = RT->GetVariable(reinterpret_cast<CHAR16*>(wname), &globalGuid, nullptr, &bufferSize, buffer);
    if (status != EFI_SUCCESS) {
        free(buffer);
        *size = 0;
        return nullptr;
    }
    *size = bufferSize;
    return buffer;
}
