//
// Created by o0kam1 on 2025/05/10.
//

#ifndef UEFI_H
#define UEFI_H

#ifdef UEFI_LIBC_USE_GNU_EFI
#include <efi.h>
#else
#define EFI_RUNTIME_SERVICES        void
#define EFI_SYSTEM_TABLE            void
#define EFI_BOOT_SERVICES           void
#define EFI_LOADED_IMAGE_PROTOCOL   void
#define EFI_HANDLE                  void*
#define EFI_FILE_HANDLE             void*
#endif

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

extern EFI_FILE_HANDLE rootfs;

void _init(EFI_HANDLE imageHandle, EFI_SYSTEM_TABLE* systemTable);
void _cleanup();

#ifdef __cplusplus
}
#endif

#endif //UEFI_H
