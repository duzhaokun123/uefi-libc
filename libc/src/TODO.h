//
// Created by o0kam1 on 2025/05/10.
//

#ifndef TODO_H
#define TODO_H

#define TODO(x) \
    do { \
        printf("TODO: %s:%d: %s\n", __FILE__, __LINE__, x); \
        abort(); \
    } while (0)

#endif //TODO_H
