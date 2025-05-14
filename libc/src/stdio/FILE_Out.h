//
// Created by o0kam1 on 2025/05/13.
//

#ifndef FILE_OUT_H
#define FILE_OUT_H
#include "FILE_stream.h"
#include <efi.h>

class FILE_Out : public FILE_stream {
public:
    explicit FILE_Out(EFI_SIMPLE_TEXT_OUT_PROTOCOL* out);
    ~FILE_Out() override = default;
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
    EFI_SIMPLE_TEXT_OUT_PROTOCOL* _out;
};

#endif //FILE_OUT_H
