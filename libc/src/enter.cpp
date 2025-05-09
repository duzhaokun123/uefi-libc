//
// Created by o0kam1 on 2025/05/09.
//

#include <uefi.h>
#include <enter.h>
#include <cstdlib>

EFI_RUNTIME_SERVICES* RT = nullptr;
EFI_SYSTEM_TABLE* ST = nullptr;
EFI_BOOT_SERVICES* BS = nullptr;
EFI_LOADED_IMAGE_PROTOCOL* LIP = nullptr;
EFI_HANDLE IM = nullptr;

EFI_STATUS EfiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable) {
    IM = imageHandle;
    ST = systemTable;
    RT = systemTable->RuntimeServices;
    BS = systemTable->BootServices;

    EFI_GUID loadedImageGuid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    BS->HandleProtocol(IM, &loadedImageGuid, reinterpret_cast<void**>(&LIP));

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
        auto argvsStart = argvs;
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
    const int ret = main(argc, argv);
    delete[] argv;
    delete[] argvs;
    return ret != 0 ? EFIERR(ret) : EFI_SUCCESS;
}
