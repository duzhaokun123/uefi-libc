//
// Created by o0kam1 on 2025/05/10.
//

#include <cerrno>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <uefi.h>
#include "FILE_File.h"
#include "FILE_In.h"
#include "FILE_Out.h"
#include "FILE_stream.h"
#include "../utils/TODO.h"
#include "../utils/utils.h"

#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS 1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS 0

#define NANOPRINTF_IMPLEMENTATION
#ifdef _MSC_VER
#undef _MSC_VER
#endif
#include "../nanoprintf.h"

EFI_FILE_HANDLE rootfs = nullptr;

int remove(const char* filename);

int rename(const char* old, const char* new_);

FILE* tmpfile();

char* tmpnam(char* s);

int fclose(FILE* stream) {
    auto r = reinterpret_cast<FILE_stream*>(stream)->fclose();
    if (r == 0) {
        delete reinterpret_cast<FILE_stream*>(stream);
    }
    return r;
}

int fflush(FILE* stream) {
    return reinterpret_cast<FILE_stream*>(stream)->fflush();
}

FILE* fopen(const char* restrict filename, const char* restrict mode) {
    bool read = false;
    bool write = false;
    bool append = false;
    bool extended = false;
    const char* p = mode;
    while (*p != '\0') {
        switch (*p) {
            case 'r': {
                read = true;
                break;
            }
            case 'w': {
                write = true;
                break;
            }
            case 'a': {
                append = true;
                break;
            }
            case '+': {
                extended = true;
                break;
            }
            default:
                break;
        }
        p++;
    }
    if ((read && write) || (read && append) || (write && append)) {
        errno = EINVAL;
        return nullptr;
    }
    if (not read && not write && not append) {
        errno = EINVAL;
        return nullptr;
    }
    if (rootfs == nullptr && LIP != nullptr) {
        EFI_GUID fileSystemGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fileSystem = nullptr;
        BS->HandleProtocol(LIP->DeviceHandle, &fileSystemGuid, reinterpret_cast<void**>(&fileSystem));
        if (fileSystem != nullptr) {
            fileSystem->OpenVolume(fileSystem, &rootfs);
        }
    }
    if (rootfs == nullptr) {
        errno = ENOENT;
        return nullptr;
    }
    filename = makeDosPath(filename);
    EFI_FILE_HANDLE fileHandle = nullptr;
    uint64_t openMode = 0;
    if (read && not extended) {
        openMode |= EFI_FILE_MODE_READ;
    } else if (write && not extended) {
        openMode |= EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE;
    } else if (append && not extended) {
        openMode |= EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE;
    } else if (read && extended) {
        openMode |= EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE;
    } else if (write && extended) {
        openMode |= EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE;
    } else if (append && extended) {
        openMode |= EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE;
    }
    auto status = rootfs->Open(rootfs, &fileHandle, (CHAR16*) filename, openMode, 0);
    /* | status                | description                                                                      |
     * |-----------------------|----------------------------------------------------------------------------------|
     * | EFI_SUCCESS           | The file was opened.                                                             |
     * | EFI_NOT_FOUND         | The specified file could not be found on the device.                             |
     * | EFI_NO_MEDIA          | The device has no medium.                                                        |
     * | EFI_MEDIA_CHANGED     | The device has a different medium in it or the medium is no longer supported.    |
     * | EFI_DEVICE_ERROR      | The device reported an error.                                                    |
     * | EFI_VOLUME_CORRUPTED  | The file system structures are corrupted.                                        |
     * | EFI_WRITE_PROTECTED   | An attempt was made to create a file, or open a file for write when the media is |
     * |                       | write-protected.                                                                 |
     * | EFI_ACCESS_DENIED     | The service denied access to the file.                                           |
     * | EFI_OUT_OF_RESOURCES  | Not enough resources were available to open the file.                            |
     * | EFI_VOLUME_FULL       | The volume is full.                                                              |
     * | EFI_INVALID_PARAMETER | This refers to a regular file, not a directory.                                  |
     */
    switch (status) {
        case EFI_SUCCESS: {
            errno = 0;
            break;
        }
        case EFI_NOT_FOUND: {
            errno = ENOENT;
            break;
        }
        case EFI_NO_MEDIA: {
            errno = ENXIO;
            break;
        }
        case EFI_MEDIA_CHANGED:
        case EFI_DEVICE_ERROR:
        case EFI_VOLUME_CORRUPTED: {
            errno = EIO;
            break;
        }
        case EFI_WRITE_PROTECTED:
        case EFI_ACCESS_DENIED: {
            errno = EACCES;
            break;
        }
        case EFI_OUT_OF_RESOURCES: {
            errno = ENOMEM;
            break;
        }
        case EFI_VOLUME_FULL: {
            errno = ENOSPC;
            break;
        }
        case EFI_INVALID_PARAMETER:
        default: {
            errno = EINVAL;
            break;
        }
    }
    delete[] filename;
    if (errno != 0) {
        return nullptr;
    }
    auto file = reinterpret_cast<FILE*>(new FILE_File(fileHandle));
    if (append) {
        fseek(file, 0, SEEK_END);
    }
    return file;
}

FILE* freopen(const char* restrict filename, const char* restrict mode, FILE* restrict stream);

void setbuf(FILE* restrict stream, char* restrict buf);

int setvbuf(FILE* restrict stream, char* restrict buf, int mode, size_t size);

int printf(const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    const auto r = vprintf(format, args);
    va_end(args);
    return r;
}

int scanf(const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    const auto r = vscanf(format, args);
    va_end(args);
    return r;
}

int snprintf(char* restrict s, size_t n, const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    const auto r = vsnprintf(s, n, format, args);
    va_end(args);
    return r;
}

int sprintf(char* restrict s, const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    const auto r = vsprintf(s, format, args);
    va_end(args);
    return r;
}

int sscanf(const char* restrict s, const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    const auto r = vsscanf(s, format, args);
    va_end(args);
    return r;
}

int vfprintf(FILE* restrict stream, const char*restrict format, va_list arg) {
    return npf_vpprintf([](int c, void* ctx) {
        static_cast<FILE_stream*>(ctx)->fputc(c);
    }, stream, format, arg);
}

int vfscanf(FILE* restrict stream, const char*restrict format, va_list arg) {
    TODO("implement vfscanf");
}

int vprintf(const char* restrict format, va_list arg) {
    return vfprintf(stdout, format, arg);
}

int vscanf(const char* restrict format, va_list arg) {
    return vfscanf(stdin, format, arg);
}

int vsnprintf(char* restrict s, size_t n, const char* restrict format, va_list arg) {
    return npf_vsnprintf(s, n, format, arg);
}

int vsprintf(char* restrict s, const char* restrict format, va_list arg) {
    return vsnprintf(s, BUFSIZ, format, arg);
}

int vsscanf(const char* restrict s, const char* restrict format, va_list arg) {
    TODO("implement vsscanf");
}

int fgetc(FILE* stream) {
    return reinterpret_cast<FILE_stream*>(stream)->fgetc();
}

char* fgets(char* restrict s, int n, FILE* restrict stream) {
    int len = 0;
    while (len < n - 1) {
        int c = reinterpret_cast<FILE_stream*>(stream)->fgetc();
        if (c == EOF) {
            break;
        }
        if (c == '\n') {
            break;
        }
        s[len] = c;
        len++;
    }
    s[len] = '\0';
    if (ferror(stream)) {
        return nullptr;
    }
    return len == 0 ? nullptr : s;
}

int fputc(int c, FILE* stream) {
    return reinterpret_cast<FILE_stream*>(stream)->fputc(c);
}

int fputs(const char* restrict s, FILE* restrict stream) {
    int len = 0;
    while (s[len] != '\0') {
        fputc(s[len], stream);
        len++;
    }
    return len;
}

int getc(FILE* stream) {
    return fgetc(stream);
}

int getchar() {
    return fgetc(stdin);
}

int putc(int c, FILE* stream) {
    return fputc(c, stream);
}

int putchar(int c) {
    return fputc(c, stdout);
}

int puts(const char* s) {
    auto len = fputs(s, stdout);
    if (len == EOF) {
        return EOF;
    }
    return len + fputc('\n', stdout);
}

int ungetc(int c, FILE* stream);

size_t fread(void* restrict ptr, size_t size, size_t nmemb, FILE* restrict stream) {
    return reinterpret_cast<FILE_stream*>(stream)->fread(ptr, size, nmemb);
}

size_t fwrite(const void* restrict ptr, size_t size, size_t nmemb, FILE* restrict stream) {
    return reinterpret_cast<FILE_stream*>(stream)->fwrite(ptr, size, nmemb);
}

// int fgetpos(FILE* restrict stream, fpos_t * restrict pos);

int fseek(FILE* stream, long int offset, int whence) {
    return reinterpret_cast<FILE_stream*>(stream)->fseek(offset, whence);
}

// int fsetpos(FILE* stream, const fpos_t* pos);

long int ftell(FILE* stream) {
    return reinterpret_cast<FILE_stream*>(stream)->ftell();
}

void rewind(FILE* stream) {
    reinterpret_cast<FILE_stream*>(stream)->fseek(0, SEEK_SET);
}

void clearerr(FILE* stream) {
    return reinterpret_cast<FILE_stream*>(stream)->clearerr();
}

int feof(FILE* stream) {
    return reinterpret_cast<FILE_stream*>(stream)->feof();
}

int ferror(FILE* stream) {
    return reinterpret_cast<FILE_stream*>(stream)->ferror();
}

void perror(const char* s) {
    printf("%s: %s\n", s, strerror(errno));
}

int fprintf(FILE* restrict stream, const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    const auto r = vfprintf(stream, format, args);
    va_end(args);
    return r;
}

int fscanf(FILE* restrict stream, const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    const auto r = vfscanf(stream, format, args);
    va_end(args);
    return r;
}

FILE* fdopen(int fd, const char* mode) {
    switch (fd) {
        case 0:
            return stdin;
        case 1:
            return stdout;
        case 2:
            return stderr;
        default:
            errno = EBADF;
            return nullptr;
    }
}

int fileno(FILE* stream) {
    if (stream == stdin) {
        return 0;
    }
    if (stream == stdout) {
        return 1;
    }
    if (stream == stderr) {
        return 2;
    }
    // XXX: maybe we can make fd work by ptr (like EFI_FILE_HANDLE), but void* is too large for int
    errno = EBADF;
    return -1;
}

FILE* fopen_in(void* in) {
    return reinterpret_cast<FILE*>(new FILE_In(static_cast<EFI_SIMPLE_TEXT_IN_PROTOCOL*>(in)));
}

FILE* fopen_out(void* out) {
    return reinterpret_cast<FILE*>(new FILE_Out(static_cast<EFI_SIMPLE_TEXT_OUT_PROTOCOL*>(out)));
}

FILE* fopen_file(void* fileHandle) {
    return reinterpret_cast<FILE*>(new FILE_File(static_cast<EFI_FILE_HANDLE>(fileHandle)));
}
