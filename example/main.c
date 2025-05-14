#include <string.h>
#include <stdio.h>

int main(int argc, char** argv) {
    char* str = "Hello, world!";
    printf("%s %llu\n", str, strlen(str));
    printf("argc: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
}
