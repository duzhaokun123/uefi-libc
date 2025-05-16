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
        setFlushErrno(status);
        return EOF;
    }
    return 0;
}

int FILE_File::fgetc() {
    char ch;
    uintn_t bufferSize = sizeof(char);
    auto status = _file->Read(_file, &bufferSize, &ch);
    if (status != EFI_SUCCESS) {
        setWriteErrno(status);
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
        setWriteErrno(status);
        return EOF;
    }
    return c;
}

size_t FILE_File::fread(void* ptr, size_t size, size_t count) {
    uintn_t bufferSize = size * count;
    auto status = _file->Read(_file, &bufferSize, ptr);
    if (status != EFI_SUCCESS) {
        setReadErrno(status);
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
        setWriteErrno(status);
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
        _errno = EIO;
        return -1;
    }
    return static_cast<long int>(position);
}

int FILE_File::feof() {
    return _isEof ? 1 : 0;
}

int FILE_File::ferror() {
    return _errno;
}

void FILE_File::clearerr() {
    _errno = 0;
}

wint_t FILE_File::fgetwc() {
    wchar_t ch;
    uintn_t bufferSize = sizeof(wchar_t);
    auto status = _file->Read(_file, &bufferSize, &ch);
    if (status != EFI_SUCCESS) {
        setReadErrno(status);
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
        setWriteErrno(status);
        return WEOF;
    }
    return c;
}

errno_t FILE_File::setFlushErrno(EFI_STATUS status) {
    /* | status               | description                               |
     * |----------------------|-------------------------------------------|
     * | EFI_SUCCESS          | The data was flushed.                     |
     * | EFI_NO_MEDIA         | The device has no medium.                 |
     * | EFI_DEVICE_ERROR     | The device reported an error.             |
     * | EFI_VOLUME_CORRUPTED | The file system structures are corrupted. |
     * | EFI_WRITE_PROTECTED  | The file or medium is write-protected.    |
     * | EFI_ACCESS_DENIED    | The file was opened read-only.            |
     * | EFI_VOLUME_FULL      | The volume is full.                       |
     */
    switch (status) {
        case EFI_SUCCESS: {
            _errno = 0;
            break;
        }
        case EFI_NO_MEDIA: {
            _errno = ENXIO;
            break;
        }
        case EFI_DEVICE_ERROR:
        case EFI_VOLUME_CORRUPTED: {
            _errno = EIO;
            break;
        }
        case EFI_WRITE_PROTECTED:
        case EFI_ACCESS_DENIED: {
            _errno = EACCES;
            break;
        }
        case EFI_VOLUME_FULL: {
            _errno = ENOSPC;
            break;
        }
        default: {
            _errno = EIO;
            break;
        }
    }
    return _errno;
}

errno_t FILE_File::setReadErrno(EFI_STATUS status) {
    /* | status               | description                                                                          |
     * |----------------------|--------------------------------------------------------------------------------------|
     * | EFI_SUCCESS          | The data was read.                                                                   |
     * | EFI_NO_MEDIA         | The device has no medium.                                                            |
     * | EFI_DEVICE_ERROR     | The device reported an error.                                                        |
     * | EFI_DEVICE_ERROR     | An attempt was made to read from a deleted file.                                     |
     * | EFI_DEVICE_ERROR     | On entry, the current file position is beyond the end of the file.                   |
     * | EFI_VOLUME_CORRUPTED | The file system structures are corrupted.                                            |
     * | EFI_BUFFER_TOO_SMALL | The BufferSize is too small to read the current directory entry. BufferSize has been |
     * |                      | updated with the size needed to complete the request.                                |
     */
    switch (status) {
        case EFI_SUCCESS: {
            _errno = 0;
            break;
        }
        case EFI_NO_MEDIA: {
            _errno = ENXIO;
            break;
        }
        case EFI_DEVICE_ERROR:
        case EFI_VOLUME_CORRUPTED: {
            _errno = EIO;
            break;
        }
        case EFI_ACCESS_DENIED: {
            _errno = EACCES;
            break;
        }
        default: {
            _errno = EIO;
            break;
        }
    }
    return _errno;
}

errno_t FILE_File::setWriteErrno(EFI_STATUS status) {
    /* | status               | description                                       |
     * |----------------------|---------------------------------------------------|
     * | EFI_SUCCESS          | The data was written.                             |
     * | EFI_UNSUPPORT        | Writes to open directory files are not supported. |
     * | EFI_NO_MEDIA         | The device has no medium.                         |
     * | EFI_DEVICE_ERROR     | The device reported an error.                     |
     * | EFI_DEVICE_ERROR     | An attempt was made to write to a deleted file.   |
     * | EFI_VOLUME_CORRUPTED | The file system structures are corrupted.         |
     * | EFI_WRITE_PROTECTED  | The file or medium is write-protected.            |
     * | EFI_ACCESS_DENIED    | The file was opened read-only.                    |
     * | EFI_VOLUME_FULL      | The volume is full.                               |
     */
    switch (status) {
        case EFI_SUCCESS: {
            _errno = 0;
            break;
        }
        case EFI_UNSUPPORTED:
        case EFI_DEVICE_ERROR:
        case EFI_VOLUME_CORRUPTED: {
            _errno = EIO;
            break;
        }
        case EFI_NO_MEDIA: {
            _errno = ENXIO;
            break;
        }

        case EFI_WRITE_PROTECTED:
        case EFI_ACCESS_DENIED: {
            _errno = EACCES;
            break;
        }
        case EFI_VOLUME_FULL: {
            _errno = ENOSPC;
            break;
        }
        default: {
            _errno = EIO;
            break;
        }
    }
    return _errno;
}
