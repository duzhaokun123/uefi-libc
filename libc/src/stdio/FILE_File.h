//
// Created by o0kam1 on 2025/05/14.
//

#ifndef FILE_FILE_H
#define FILE_FILE_H
#include "FILE_stream.h"
#include <efi.h>

#include "errno.h"

class FILE_File : public FILE_stream {
public:
    explicit FILE_File(EFI_FILE_HANDLE fileHandle);
    ~FILE_File() override = default;
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
    errno_t setFlushErrno(EFI_STATUS status);
    errno_t setReadErrno(EFI_STATUS status);
    errno_t setWriteErrno(EFI_STATUS status);

    EFI_FILE_HANDLE _file;
    errno_t _errno = 0;
    bool _isEof = false;
};

#endif //FILE_FILE_H
