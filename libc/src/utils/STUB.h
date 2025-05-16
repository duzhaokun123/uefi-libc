//
// Created by o0kam1 on 2025/05/16.
//

#ifndef STUB_H
#define STUB_H

#include <stdio.h>

#ifdef UEFI_LIBC_STUB_AS_TRAP
#define FUNC_STUB(func_decl) \
    func_decl { \
        printf("STUB: %s\n", #func_decl); \
        __builtin_trap(); \
    }
#elifdef UEFI_LIBC_STUB_AS_EMPTY
#warning "no return value can cause undefined behavior"
#define FUNC_STUB(func_decl) \
    func_decl { \
        printf("STUB: %s\n", #func_decl); \
    }
#else
#define FUNC_STUB(func_decl) \
    func_decl;
#endif

#endif //STUB_H
