//
// Created by o0kam1 on 2025/05/09.
//

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cwchar>

int main(int argc, char** argv) {
    printf("hello world!\n");
    printf("%d %f %c %s\n", 123, 4.56, 'a', "ss");
    fprintf(stderr, "hello stderr!\n");
    printf("nullptr: %p, stdout: %p\n", nullptr, stdout);
    printf("%d:\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }

    putws(L"wchar: [!!! εÐiţ Þr0ģЯãm səTτıИğ§ !!!]");

    uint8_t bits = 0b1;
    while (bits != 0) {
        printf("%08b\n", bits);
        bits = bits << 1;
    }

    auto a = malloc(1024 * 1024);
    if (a == nullptr) {
        printf("1024^2 malloc failed\n");
    } else {
        printf("1024^2 malloc success\n");
        a = realloc(a, 8);
        printf("realloc %p\n", a);
    }
    free(a);

    const char* filename = "\\test.bin";
    uint8_t data[16] = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F
    };
    FILE* file = fopen(filename, "w");
    if (file == nullptr) {
        printf("failed to open file: %s\n", strerror(errno));
    } else {
        printf("try to write to %s\n", filename);
        size_t r = fwrite(data, sizeof(data), 1, file);
        printf("write %zu block to %s\n", r, filename);
        fclose(file);
    }
    file = fopen(filename, "r");
    if (file == nullptr) {
        printf("failed to open file: %s\n", strerror(errno));
    } else {
        printf("try to read from %s\n", filename);
        uint8_t buf[16];
        size_t r = fread(buf, sizeof(buf), 1, file);
        if (r == 0) {
            printf("failed to read file: %s\n", strerror(errno));
        } else {
            printf("read %zu block from %s\n", r, filename);
            for (const uint8_t i : buf) {
                printf("%02X ", i);
            }
            printf("\n");
        }
        fclose(file);
    }
}
