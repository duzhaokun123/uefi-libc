#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    auto str = "Hello, world!";
    printf("%s %zu\n", str, strlen(str));
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
}
