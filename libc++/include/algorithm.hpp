#pragma once

namespace std {
    template<class T>
    const T& max(const T& a, const T& b) {
        return a > b ? a : b;
    }
    template<class T>
    const T& min(const T& a, const T& b) {
        return a < b ? a : b;
    }
}
