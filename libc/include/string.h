//
// Created by o0kam1 on 2025/05/10.
//

#ifndef STRING_H
#define STRING_H

#include <stddef.h>
#include <restrict.h>

#ifdef __cplusplus
extern "C" {
#endif

void* memcpy(void* restrict s1, const void* restrict s2, size_t n);
void* memccpy(void* restrict s1, const void* restrict s2, int c, size_t n);
void* memmove(void* s1, const void* s2, size_t n);
char* strcpy(char* restrict s1, const char* restrict s2);
char* strncpy(char* restrict s1, const char* restrict s2, size_t n);
char* strdup(const char* s);
char* strndup(const char* s, size_t n);
char* strcat(char* restrict s1, const char* restrict s2);
char* strncat(char* restrict s1, const char* restrict s2, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);
int strcmp(const char* s1, const char* s2);
int strcoll(const char* s1, const char* s2);
int strncmp(const char* s1, const char* s2, size_t n);
size_t strxfrm(char* restrict s1, const char* restrict s2, size_t n);
void* memchr(const void* s, int c, size_t n);
char* strchr(const char* s, int c);
size_t strcspn(const char* s1, const char* s2);
char* strpbrk(const char* s1, const char* s2);
char* strrchr(const char* s, int c);
size_t strspn(const char* s1, const char* s2);
char* strstr(const char* s1, const char* s2);
char* strtok(char* restrict s1, const char* restrict s2);
void* memset(void* s, int c, size_t n);
void* memset_explicit(void* s, int c, size_t n);
char* strerror(int errnum);
size_t strlen(const char* s);
size_t strnlen(const char* s, size_t n);

// posix
void* memmem(const void* haystack, size_t haystacklen,
             const void* needle, size_t needlelen);
#ifdef __cplusplus
}
#endif

#endif //STRING_H
