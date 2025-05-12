//
// Created by o0kam1 on 2025/05/10.
//

#include <cstdio>
#include <cstdlib>
#include <uefi.h>
#include <FILE.h>

static_assert(sizeof(int8_t) == 1, "int8_t is not 1 byte");
static_assert(sizeof(int16_t) == 2, "int16_t is not 2 bytes");
static_assert(sizeof(int32_t) == 4, "int32_t is not 4 bytes");
static_assert(sizeof(int64_t) == 8, "int64_t is not 8 bytes");
static_assert(sizeof(uint8_t) == 1, "uint8_t is not 1 byte");
static_assert(sizeof(uint16_t) == 2, "uint16_t is not 2 bytes");
static_assert(sizeof(uint32_t) == 4, "uint32_t is not 4 bytes");
static_assert(sizeof(uint64_t) == 8, "uint64_t is not 8 bytes");
static_assert(sizeof(size_t) == sizeof(void*), "size_t is not the same size as void*");
static_assert(sizeof(wchar_t) == 2, "wchar_t is not 2 bytes");
static_assert(sizeof(char) == 1, "char is not 1 byte");
static_assert(sizeof(wchar_t) == sizeof(CHAR16), "wchar_t is not the same size as CHAR16");

EFI_RUNTIME_SERVICES* RT = nullptr;
EFI_SYSTEM_TABLE* ST = nullptr;
EFI_BOOT_SERVICES* BS = nullptr;
EFI_LOADED_IMAGE_PROTOCOL* LIP = nullptr;
EFI_HANDLE IM = nullptr;

FILE* _stdout = nullptr;
FILE* _stderr = nullptr;
FILE* _stdin = nullptr;

int _argc = 0;
char** _argv = nullptr;
char* _argvs = nullptr;

#if defined(UEFI_CLANG_FAKE_WIN32) && defined(__x86_64__)
extern "C" {
uint64_t _fltused; // pass float on x86_64
}
#endif

void _init(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable) {
    IM = imageHandle;
    ST = systemTable;
    RT = systemTable->RuntimeServices;
    BS = systemTable->BootServices;

    EFI_GUID loadedImageGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    BS->HandleProtocol(IM, &loadedImageGuid, reinterpret_cast<void**>(&LIP));

    _stdout = new FILE{
        .type = FILE_STREAM_TYPE_OUT,
        .stream = {
            .text_out = ST->ConOut,
        },
    };
    _stderr = new FILE{
        .type = FILE_STREAM_TYPE_OUT,
        .stream = {
            .text_out = ST->StdErr,
        },
    };
    _stdin = new FILE{
        .type = FILE_STREAM_TYPE_IN,
        .stream = {
            .text_in = ST->ConIn
        },
    };

    int argc = 0;
    wchar_t** argvw = nullptr;
    EFI_GUID shellParamGuid = EFI_SHELL_PARAMETERS_PROTOCOL_GUID;
    EFI_SHELL_PARAMETERS_PROTOCOL* shellParam = nullptr;
    EFI_STATUS status = BS->HandleProtocol(IM, &shellParamGuid, reinterpret_cast<void**>(&shellParam));
    if (status == EFI_SUCCESS) {
        argc = shellParam->Argc;
        argvw = reinterpret_cast<wchar_t**>(shellParam->Argv);
    } else {
        // TODO: support shell 1.0
    }
    char** argv = nullptr;
    char* argvs = nullptr;
    if (argc != 0 && argvw != nullptr) {
        argv = new char*[argc + 1];
        size_t argvSize = 0;
        for (size_t i = 0; i < argc; i++) {
            for (size_t j = 0; argvw[i] != nullptr && argvw[i][j] != L'\0'; j++) {
                argvSize += argvw[i][j] < 0x80 ? 1 : argvw[i][j] < 0x800 ? 2 : 3;
            }
            argvSize += 1; // for null terminator
        }
        argvs = new char[argvSize];
        for (size_t i = 0; i < argc; i++) {
            argv[i] = argvs;
            auto len = wcstombs(argv[i], argvw[i], argvSize);
            if (len == -1) {
                continue;
            }
            argvs += len;
            argvs += 1; // for null terminator
        }
        argv[argc] = nullptr;
    }
    _argc = argc;
    _argv = argv;
    _argvs = argvs;
}

void _cleanup() {
    delete[] _argv;
    delete[] _argvs;
    delete _stdout;
    delete _stderr;
    delete _stdin;

    if (rootfs != nullptr) {
        rootfs->Close(rootfs);
    }
}
