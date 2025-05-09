//
// Created by o0kam1 on 2025/05/10.
//

#include <new>
#include <cstdlib>

void* operator new(const std::size_t size) {
    return malloc(size);
}

void* operator new[](const std::size_t size) {
    return malloc(size);
}

void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete(void* ptr, std::size_t) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr, std::size_t) noexcept {
    free(ptr);
}
