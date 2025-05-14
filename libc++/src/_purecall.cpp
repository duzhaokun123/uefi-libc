//
// Created by o0kam1 on 2025/05/14.
//

#include <cstdio>
#include <cstdlib>


extern "C" {
    [[noreturn]]
    int _purecall();
}

[[noreturn]]
int _purecall() {
    printf("_purecall() called\n");
    abort();
}
