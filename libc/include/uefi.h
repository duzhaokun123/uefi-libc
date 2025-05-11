//
// Created by o0kam1 on 2025/05/10.
//

#ifndef UEFI_H
#define UEFI_H

#include <efi.h>

#ifdef __cplusplus
extern "C" {
#endif

extern EFI_RUNTIME_SERVICES* RT;
#define gRT RT
extern EFI_SYSTEM_TABLE* ST;
#define gST ST
extern EFI_BOOT_SERVICES* BS;
#define gBS BS
extern EFI_LOADED_IMAGE_PROTOCOL* LIP;
extern EFI_HANDLE IM;

typedef INTN intn_t;
typedef UINTN uintn_t;

extern EFI_FILE_HANDLE rootfs;

extern int _argc;
extern char** _argv;
extern char* _argvs;

void _init(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable);
void _cleanup();

#ifdef __cplusplus
}
#endif

#endif //UEFI_H
