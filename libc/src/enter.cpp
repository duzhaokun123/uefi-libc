//
// Created by o0kam1 on 2025/05/09.
//

#include <uefi.h>
#include <enter.h>
#include <cstdlib>
#include <cstdio>

EFI_STATUS EfiMain(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable) {
    _init(imageHandle, systemTable);
    const int ret = main(_argc, _argv);
    _cleanup();
    return ret != 0 ? EFIERR(ret) : EFI_SUCCESS;
}
