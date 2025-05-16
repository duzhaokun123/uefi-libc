//
// Created by o0kam1 on 2025/05/13.
//

#ifndef FILE_IN_H
#define FILE_IN_H

#include "FILE_stream.h"
#include <efi.h>
#include <errno.h>

class FILE_In : public FILE_stream {
public:
    explicit FILE_In(EFI_SIMPLE_TEXT_IN_PROTOCOL* in);
    ~FILE_In() override = default;
    int fclose() override;
    int fflush() override;
    int fgetc() override;
    int fputc(int c) override;
    size_t fread(void* restrict ptr, size_t size, size_t count) override;
    size_t fwrite(const void* restrict ptr, size_t size, size_t count) override;
    int fseek(long offset, int whence) override;
    long int ftell() override;
    int feof() override;
    int ferror() override;
    void clearerr() override;

    wint_t fgetwc() override;
    wint_t fputwc(wchar_t c) override;

private:
    errno_t _error = 0;
    EFI_SIMPLE_TEXT_IN_PROTOCOL* _in;
};

#endif //FILE_IN_H
