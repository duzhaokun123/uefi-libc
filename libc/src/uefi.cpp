//
// Created by o0kam1 on 2025/05/10.
//

#include <cstdio>
#include <cstdlib>
#include <uefi.h>

#include "stdio/FILE_In.h"
#include "stdio/FILE_Out.h"

static_assert(sizeof(wchar_t) == sizeof(CHAR16), "sizeof(wchar_t) != sizeof(CHAR16)");

EFI_RUNTIME_SERVICES* RT = nullptr;
EFI_SYSTEM_TABLE* ST = nullptr;
EFI_BOOT_SERVICES* BS = nullptr;
EFI_LOADED_IMAGE_PROTOCOL* LIP = nullptr;
EFI_HANDLE IM = nullptr;

FILE* _stdout = nullptr;
FILE* _stderr = nullptr;
FILE* _stdin = nullptr;

int argc = 0;
char** argv = nullptr;
char* argvs = nullptr;

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

    _stdout = reinterpret_cast<FILE*>(new FILE_Out(ST->ConOut));
    _stderr = reinterpret_cast<FILE*>(new FILE_Out(ST->StdErr));
    _stdin = reinterpret_cast<FILE*>(new FILE_In(ST->ConIn));

    argc = 0;
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
    argv = nullptr;
    argvs = nullptr;
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
}

void _cleanup() {
    // TODO: track opened files and close them
    delete[] argv;
    delete[] argvs;
    delete _stdout;
    delete _stderr;
    delete _stdin;

    if (rootfs != nullptr) {
        rootfs->Close(rootfs);
    }
}
