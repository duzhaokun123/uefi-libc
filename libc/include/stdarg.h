//
// Created by o0kam1 on 2025/05/10.
//

#ifndef STDARG_H
#define STDARG_H

#define __STDC_VERSION_STDARG_H__ 202311L

typedef __builtin_va_list va_list;
#define va_start(v,l) __builtin_va_start(v,l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v,l) __builtin_va_arg(v,l)
#define va_copy(d,s) __builtin_va_copy(d,s)

#endif //STDARG_H
