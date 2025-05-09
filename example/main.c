//
// Created by o0kam1 on 2025/05/09.
//

#include <stdio.h>

int main(int argc, char** argv) {
    printf("%d:\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
}
