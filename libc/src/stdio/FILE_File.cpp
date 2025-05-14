//
// Created by o0kam1 on 2025/05/14.
//

#include "FILE_File.h"

#include <cstdint>
#include <cstdio>
#include <cstdlib>

FILE_File::FILE_File(EFI_FILE_HANDLE fileHandle) {
    _file = fileHandle;
}

int FILE_File::fclose() {
    auto status = _file->Close(_file);
    if (status != EFI_SUCCESS) {
        return EOF;
    }
    return 0;
}

int FILE_File::fflush() {
    auto status = _file->Flush(_file);
    if (status != EFI_SUCCESS) {
        _status = status;
        return EOF;
    }
    return 0;
}

int FILE_File::fgetc() {
    char ch;
    uintn_t bufferSize = sizeof(char);
    auto status = _file->Read(_file, &bufferSize, &ch);
    if (status != EFI_SUCCESS) {
        _status = status;
        return EOF;
    }
    if (bufferSize == 0) {
        _isEof = true;
        return EOF;
    }
    return ch;
}

int FILE_File::fputc(int c) {
    char ch = c;
    uintn_t bufferSize = sizeof(char);
    auto status = _file->Write(_file, &bufferSize, &ch);
    if (status != EFI_SUCCESS) {
        _status = status;
        return EOF;
    }
    return c;
}

size_t FILE_File::fread(void* ptr, size_t size, size_t count) {
    uintn_t bufferSize = size * count;
    auto status = _file->Read(_file, &bufferSize, ptr);
    if (status != EFI_SUCCESS) {
        _status = status;
        return 0;
    }
    if (bufferSize == 0) {
        _isEof = true;
    }
    // FIXME: bufferSize readed may not n times of size
    return bufferSize / size;
}

size_t FILE_File::fwrite(const void* ptr, size_t size, size_t count) {
    uintn_t bufferSize = size * count;
    auto status = _file->Write(_file, &bufferSize, const_cast<void*>(ptr));
    if (status != EFI_SUCCESS) {
        _status = status;
        return 0;
    }
    // FIXME: bufferSize writed may not n times of size
    return bufferSize / size;
}

int FILE_File::fseek(long offset, int whence) {
    switch (whence) {
        case SEEK_SET: {
            auto status = _file->SetPosition(_file, offset);
            if (status != EFI_SUCCESS) {
                return -1;
            }
            _isEof = false;
            return 0;
        }
        case SEEK_CUR: {
            auto currentPosition = ftell();
            if (currentPosition != 0) {
                return -1;
            }
            auto status = _file->SetPosition(_file, currentPosition + offset);
            if (status != EFI_SUCCESS) {
                return -1;
            }
            _isEof = false;
            return 0;
        }
        case SEEK_END: {
            EFI_GUID fileInfoGuid = EFI_FILE_INFO_ID;
            uintn_t bufferSize = 0;
            auto status = _file->GetInfo(_file, &fileInfoGuid, &bufferSize, nullptr);
            auto fileInfo = static_cast<EFI_FILE_INFO*>(malloc(bufferSize));
            status = _file->GetInfo(_file, &fileInfoGuid, &bufferSize, &fileInfo);
            if (status != EFI_SUCCESS) {
                free(fileInfo);
                return -1;
            }
            auto fileSize = fileInfo->FileSize;
            free(fileInfo);
            status = _file->SetPosition(_file, fileSize + offset);
            if (status != EFI_SUCCESS) {
                return -1;
            }
            _isEof = false;
            return 0;
        }
        default:
            return -1;
    }
}

long int FILE_File::ftell() {
    uint64_t position = 0;
    auto status = _file->GetPosition(_file, &position);
    if (status != EFI_SUCCESS) {
        _status = status;
        return -1;
    }
    return static_cast<long int>(position);
}

int FILE_File::feof() {
    return _isEof ? 1 : 0;
}

int FILE_File::ferror() {
    // XXX: should we convert EFI_STATUS to errno?
    return _status != EFI_SUCCESS ? 1 : 0;
}

void FILE_File::clearerr() {
    _status = EFI_SUCCESS;
}

wint_t FILE_File::fgetwc() {
    wchar_t ch;
    uintn_t bufferSize = sizeof(wchar_t);
    auto status = _file->Read(_file, &bufferSize, &ch);
    if (status != EFI_SUCCESS) {
        _status = status;
        return WEOF;
    }
    if (bufferSize == 0) {
        _isEof = true;
        return WEOF;
    }
    return ch;
}

wint_t FILE_File::fputwc(wchar_t c) {
    wchar_t ch = c;
    uintn_t bufferSize = sizeof(wchar_t);
    auto status = _file->Write(_file, &bufferSize, &ch);
    if (status != EFI_SUCCESS) {
        _status = status;
        return WEOF;
    }
    return c;
}
