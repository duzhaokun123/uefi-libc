//
// Created by o0kam1 on 2025/05/13.
//

#ifndef FILE_H
#define FILE_H

#include <restrict.h>
#include <cwchar>

class FILE_stream {
public:
    virtual ~FILE_stream() = default;
    virtual int fclose() = 0;
    virtual int fflush() = 0;
    // virtual void setbuf(char* restrict buf);
    // virtual int setvbuf(char* restrict buf, int mode, size_t size);
    virtual int fgetc() = 0;
    virtual int fputc(int c) = 0;
    virtual size_t fread(void* restrict ptr, size_t size, size_t count) = 0;
    virtual size_t fwrite(const void* restrict ptr, size_t size, size_t count) = 0;
    virtual int fseek(long offset, int whence) = 0;
    virtual long int ftell() = 0;
    virtual int feof() = 0;
    virtual int ferror() = 0;
    virtual void clearerr() = 0;

    // wchar.h
    virtual wint_t fgetwc() = 0;
    virtual wint_t fputwc(wchar_t c) = 0;
};

#endif //FILE_H
