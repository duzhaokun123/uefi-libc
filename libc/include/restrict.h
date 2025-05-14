//
// Created by o0kam1 on 2025/05/10.
//

#ifndef RESTRICT_H
#define RESTRICT_H

#if __has_builtin(__restrict)
#define restrict __restrict
#else
#define restrict
#endif

#endif //RESTRICT_H
