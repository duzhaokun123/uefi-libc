//
// Created by o0kam1 on 2025/05/10.
//

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <uefi.h>
#include "FILE.h"
#include "TODO.h"
#include "utils.h"

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
#include "nanoprintf.h"

EFI_FILE_HANDLE rootfs = nullptr;

int text_out_putc(int c, EFI_SIMPLE_TEXT_OUT_PROTOCOL* out) {
    if (c == '\n') {
        text_out_putc('\r', out);
    }
    CHAR16 chr[] = {static_cast<CHAR16>(c), L'\0'};
    auto status = out->OutputString(out, chr);
    if (status != EFI_SUCCESS) {
        // TODO: handle error
    }
    return c;
}

int file_putc(int c, EFI_FILE_HANDLE file) {
    // TODO:
    TODO("Implement file_putc");
}

int fputc(int c, FILE* stream) {
    errno = 0;
    switch (stream->type) {
        case FILE_STREAM_TYPE_IN:
        case FILE_STREAM_TYPE_IN_EX: {
            errno = EIO;
            return EOF;
        }
        case FILE_STREAM_TYPE_OUT: {
            return text_out_putc(c, stream->stream.text_out);
        }
        case FILE_STREAM_TYPE_FILE: {
            return file_putc(c, stream->stream.file);
        }
        default: {
            errno = EINVAL;
            return EOF;
        }
    }
}

int printf(const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    const auto r = vprintf(format, args);
    va_end(args);
    return r;
}

int vprintf(const char* restrict format, va_list arg) {
    return vfprintf(stdout, format, arg);
}

int fprintf(FILE* restrict stream, const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    const auto r = vfprintf(stream, format, args);
    va_end(args);
    return r;
}

int vfprintf(FILE* restrict stream, const char*restrict format, va_list arg) {
    return npf_vpprintf([](int c, void* ctx) { fputc(c, static_cast<FILE*>(ctx)); }, stream, format, arg);
}

int sprintf(char* restrict s, const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    const auto r = vsprintf(s, format, args);
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

int vsprintf(char* restrict s, const char* restrict format, va_list arg) {
    return vsnprintf(s, BUFSIZ, format, arg);
}

int vsnprintf(char* restrict s, size_t n, const char* restrict format, va_list arg) {
    return npf_vsnprintf(s, n, format, arg);
}

int text_in_getc(EFI_SIMPLE_TEXT_IN_PROTOCOL* in) {
    uintn_t index = 0;
    BS->WaitForEvent(1, &in->WaitForKey, &index);
    EFI_INPUT_KEY key;
    auto status = in->ReadKeyStroke(in, &key);
    if (status != EFI_SUCCESS) {
        return 0; // why not EOF?
    }
    return key.UnicodeChar;
}

int text_in_ex_getc(EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL* in) {
    uintn_t index = 0;
    BS->WaitForEvent(1, &in->WaitForKeyEx, &index);
    EFI_KEY_DATA key;
    auto status = in->ReadKeyStrokeEx(in, &key);
    if (status != EFI_SUCCESS) {
        return 0; // why not EOF?
    }
    return key.Key.UnicodeChar;
}

int file_getc(EFI_FILE_HANDLE file) {
    // TODO:
    TODO("Implement file_getc");
}

int fgetc(FILE* stream) {
    errno = 0;
    switch (stream->type) {
        case FILE_STREAM_TYPE_OUT: {
            errno = EIO;
            return EOF;
        }
        case FILE_STREAM_TYPE_IN: {
            return text_in_getc(stream->stream.text_in);
        }
        case FILE_STREAM_TYPE_IN_EX: {
            return text_in_ex_getc(stream->stream.text_in_ex);
        }
        case FILE_STREAM_TYPE_FILE: {
            return file_getc(stream->stream.file);
        }
        default: {
            errno = EINVAL;
            return EOF;
        }
    }
}

int getchar() {
    return fgetc(stdin);
}

FILE* fopen(const char* restrict filename, const char* restrict mode) {
    errno = 0;
    bool read = false;
    bool write = false;
    bool append = false;
    bool extended = false;

    const char* modePtr = mode;
    while (*modePtr != '\0') {
        switch (*modePtr) {
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
        modePtr++;
    }

    if ((read && write) || (read && append) || (write && append)) {
        errno = EINVAL;
        return nullptr;
    }
    if (not read && not write && not append) {
        errno = EINVAL;
        return nullptr;
    }

    filename = makeDosPath(filename);
    auto file = new FILE;
    if (strcmp(filename, "\\dev\\stdin") == 0) {
        file->type = FILE_STREAM_TYPE_IN;
        file->stream.text_in = ST->ConIn;
        return file;
    }
    if (strcmp(filename, "\\dev\\stdout") == 0) {
        file->type = FILE_STREAM_TYPE_OUT;
        file->stream.text_out = ST->ConOut;
        return file;
    }
    if (strcmp(filename, "\\dev\\stderr") == 0) {
        file->type = FILE_STREAM_TYPE_OUT;
        file->stream.text_out = ST->StdErr;
        return file;
    }

    EFI_STATUS status;
    if (rootfs == nullptr && LIP != nullptr) {
        EFI_GUID fileSystemProtocolGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
        EFI_SIMPLE_FILE_SYSTEM_PROTOCOL* fileSystemProtocol;
        status = BS->HandleProtocol(LIP->DeviceHandle, &fileSystemProtocolGuid, reinterpret_cast<void**>(&fileSystemProtocol));
        if (status == EFI_SUCCESS) {
            fileSystemProtocol->OpenVolume(fileSystemProtocol, &rootfs);
        }
    }
    if (rootfs == nullptr) {
        errno = ENODEV;
        delete file;
        return nullptr;
    }

    file->type = FILE_STREAM_TYPE_FILE;
    uint64_t openMode = 0;
    if (read) openMode |= EFI_FILE_MODE_READ;
    if (write || append) openMode |= EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE | EFI_FILE_MODE_CREATE;
    auto wfilename = new wchar_t[strlen(filename)];
    mbstowcs(wfilename, filename, strlen(filename));
    delete[] filename;
    status = rootfs->Open(rootfs, &file->stream.file, reinterpret_cast<CHAR16*>(wfilename), openMode, 0);
    delete[] wfilename;
    if (status != EFI_SUCCESS) {
        switch (status) {
            // https://uefi.org/specs/UEFI/2.11/13_Protocols_Media_Access.html#efi-file-protocol-open
            case EFI_NOT_FOUND: {
                errno = ENOENT;
                break;
            }
            case EFI_DEVICE_ERROR: {
                errno = EIO;
                break;
            }
            case EFI_ACCESS_DENIED:
            case EFI_WRITE_PROTECTED: {
                errno = EACCES;
                break;
            }
            case EFI_VOLUME_FULL: {
                errno = ENOSPC;
                break;
            }
            default: {
                errno = EIO;
                break;
            }
        }
        delete file;
        return nullptr;
    }
    if (append) {
        fseek(file, 0, SEEK_END);
    }
    // TODO: extended

    return file;
}

int fclose(FILE* stream) {
    int ret = 0;
    if (stream->type == FILE_STREAM_TYPE_FILE) {
        auto file = stream->stream.file;
        auto status = file->Close(file);
        if (status != EFI_SUCCESS) {
            ret = EOF;
        }
    }
    if (stream == stdin || stream == stdout || stream == stderr) {
        ret = EOF;
    } else {
        delete stream;
    }
    return ret;
}

int fseek(FILE* stream, long int offset, int whence) {
    errno = 0;
    if (stream->type != FILE_STREAM_TYPE_FILE) {
        errno = ESPIPE;
        return -1;
    }
    if (whence != SEEK_SET && whence != SEEK_CUR && whence != SEEK_END) {
        errno = EINVAL;
        return -1;
    }

    auto file = stream->stream.file;
    EFI_STATUS status = EFI_SUCCESS;
    switch (whence) {
        case SEEK_END: {
            EFI_GUID fileInfoGuid = EFI_FILE_INFO_ID;
            uintn_t fileInfoSize = 0;
            EFI_FILE_INFO* fileInfo;
            file->GetInfo(file, &fileInfoGuid, &fileInfoSize, nullptr);
            fileInfo = static_cast<EFI_FILE_INFO*>(malloc(fileInfoSize));
            status = file->GetInfo(file, &fileInfoGuid, &fileInfoSize, fileInfo);
            if (status == EFI_SUCCESS) {
                offset += fileInfo->FileSize;
                status = file->SetPosition(file, offset);
            }
            free(fileInfo);
            break;
        }
        case SEEK_SET: {
            status = file->SetPosition(file, offset);
            break;
        }
        case SEEK_CUR: {
            uint64_t pos;
            status = file->GetPosition(file, &pos);
            if (status == EFI_SUCCESS) {
                file->SetPosition(file, pos + offset);
            }
            break;
        }
    }
    if (status != EFI_SUCCESS) {
        return -1;
    }
    return 0;
}

long int ftell(FILE* stream) {
    if (stream->type != FILE_STREAM_TYPE_FILE) {
        errno = ESPIPE;
        return -1;
    }
    auto file = stream->stream.file;
    uint64_t pos;
    auto status = file->GetPosition(file, &pos);
    if (status != EFI_SUCCESS) {
        errno = EIO;
        return -1;
    }
    return static_cast<long int>(pos);
}

size_t fread(void* restrict ptr, size_t size, size_t nmemb, FILE* restrict stream) {
    errno = 0;
    if (stream->type != FILE_STREAM_TYPE_FILE) {
        errno = ESPIPE; // TODO: support non file direct io
        return 0;
    }
    if (ptr == nullptr || size == 0 || nmemb == 0) {
        return 0;
    }
    auto file = stream->stream.file;
    uintn_t bufSize = size * nmemb;
    auto status = file->Read(file, &bufSize, ptr);
    if (status != EFI_SUCCESS) {
        stream->status = EFI_ERROR(status);
        return 0;
    }
    return bufSize / size;
}

size_t fwrite(const void* restrict ptr, size_t size, size_t nmemb, FILE* restrict stream) {
    errno = 0;
    if (stream->type != FILE_STREAM_TYPE_FILE) {
        errno = ESPIPE; // TODO: support non file direct io
        return 0;
    }
    if (ptr == nullptr || size == 0 || nmemb == 0) {
        return 0;
    }
    auto file = stream->stream.file;
    uintn_t bufSize = size * nmemb;
    auto status = file->Write(file, &bufSize, const_cast<void*>(ptr));
    if (status != EFI_SUCCESS) {
        stream->status = EFI_ERROR(status);
        return 0;
    }
    return bufSize / size;
}

int fputs(const char* restrict s, FILE* restrict stream) {
    const char* p = s;
    int count = 0;
    while (*p != '\0') {
        if (fputc(*p, stream) == EOF) {
            return EOF;
        }
        p++;
        count++;
    }
    return count;
}

int puts(const char* s) {
    auto r = fputs(s, stdout);
    if (r == EOF) {
        return EOF;
    }
    fputc('\n', stdout);
    return r + 1;
}

int putchar(int c) {
    return putc(c, stdout);
}

int putc(int c, FILE* stream) {
    return fputc(c, stream);
}

FILE* fdopen(int fd, const char* mode) {
    errno = 0;
    switch (fd) {
        case 0: {
            return fopen("/dev/stdin", mode);
        }
        case 1: {
            return fopen("/dev/stdout", mode);
        }
        case 2: {
            return fopen("/dev/stderr", mode);
        }
        default: {
            errno = EBADF;
            return nullptr;
        }
    }
}

int fileno(FILE* stream) {
    errno = 0;
    if (stream == stdin) {
        return 0;
    }
    if (stream == stdout) {
        return 1;
    }
    if (stream == stderr) {
        return 2;
    }
    errno = EBADF;
    return -1;
}

void perror(const char* s) {
    fprintf(stderr, "%s: %s", s, strerror(errno));
}

int ferror(FILE* stream) {
    auto state = stream->status;
    if (state == EFI_SUCCESS) {
        return 0;
    } else {
        return EFI_ERROR(state);
    }
}
