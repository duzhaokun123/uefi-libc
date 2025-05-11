//
// Created by o0kam1 on 2025/05/10.
//

#ifndef STDIO_H
#define STDIO_H

#include <restrict.h>
#include <stdarg.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct FILE;

#ifndef __cplusplus
typedef struct FILE FILE;
#endif

extern FILE* _stdout;
extern FILE* _stderr;
extern FILE* _stdin;

#define stdout (_stdout)
#define stderr (_stderr)
#define stdin  (_stdin)

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#ifndef BUFSIZ
#define BUFSIZ 8192
#endif

#define EOF (-1)

int remove(const char* filename);
int rename(const char* old, const char* new_);
FILE* tmpfile(void);
char* tmpnam(char* s);
int fclose(FILE* stream);
int fflush(FILE* stream);
FILE* fopen(const char* restrict filename, const char* restrict mode);
FILE* freopen(const char* restrict filename, const char* restrict mode,
              FILE* restrict stream);
void setbuf(FILE* restrict stream, char* restrict buf);
int setvbuf(FILE* restrict stream, char* restrict buf, int mode, size_t size);
int printf(const char* restrict format, ...);
int scanf(const char* restrict format, ...);
int snprintf(char* restrict s, size_t n, const char* restrict format, ...);
int sprintf(char* restrict s, const char* restrict format, ...);
int sscanf(const char* restrict s, const char* restrict format, ...);
int vfprintf(FILE* restrict stream, const char*restrict format, va_list arg);
int vfscanf(FILE* restrict stream, const char*restrict format, va_list arg);
int vprintf(const char* restrict format, va_list arg);
int vscanf(const char* restrict format, va_list arg);
int vsnprintf(char* restrict s, size_t n, const char* restrict format, va_list arg);
int vsprintf(char* restrict s, const char* restrict format, va_list arg);
int vsscanf(const char* restrict s, const char* restrict format, va_list arg);
int fgetc(FILE* stream);
char* fgets(char* restrict s, int n, FILE* restrict stream);
int fputc(int c, FILE* stream);
int fputs(const char* restrict s, FILE* restrict stream);
int getc(FILE* stream);
int getchar(void);
int putc(int c, FILE* stream);
int putchar(int c);
int puts(const char* s);
int ungetc(int c, FILE* stream);
size_t fread(void* restrict ptr, size_t size, size_t nmemb,
             FILE* restrict stream);
size_t fwrite(const void* restrict ptr, size_t size, size_t nmemb,
              FILE* restrict stream);
// int fgetpos(FILE* restrict stream, fpos_t * restrict pos);
int fseek(FILE* stream, long int offset, int whence);
// int fsetpos(FILE* stream, const fpos_t* pos);
long int ftell(FILE* stream);
void rewind(FILE* stream);
void clearerr(FILE* stream);
int feof(FILE* stream);
int ferror(FILE* stream);
void perror(const char* s);
int fprintf(FILE* restrict stream, const char* restrict format, ...);
int fscanf(FILE* restrict stream, const char* restrict format, ...);

#ifdef __cplusplus
}
#endif

#endif //STDIO_H
