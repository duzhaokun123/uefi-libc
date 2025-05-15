//
// Created by o0kam1 on 2025/05/09.
//

#include <uefi.h>

extern int main(int argc, char** argv);

extern int argc;
extern char** argv;

EFI_STATUS EfiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable) {
    _init(imageHandle, systemTable);
    const int ret = main(argc, argv);
    _cleanup();
    return ret != 0 ? EFIERR(ret) : EFI_SUCCESS;
}
