//
// Created by o0kam1 on 2025/05/12.
//

#ifndef FILE_H
#define FILE_H

#include <stdio.h>
#include <efi.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef enum FILE_STREAM_TYPE {
        FILE_STREAM_TYPE_FILE,  // EFI_FILE_HANDLE
        FILE_STREAM_TYPE_OUT,   // EFI_SIMPLE_TEXT_OUT_PROTOCOL
        FILE_STREAM_TYPE_IN,    // EFI_SIMPLE_TEXT_IN_PROTOCOL
        FILE_STREAM_TYPE_IN_EX, // EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL
        // TODO: fs, block ...
    } FILE_STREAM_TYPE;

struct FILE {
    FILE_STREAM_TYPE type;
    union {
        EFI_FILE_HANDLE file;
        EFI_SIMPLE_TEXT_OUT_PROTOCOL* text_out;
        EFI_SIMPLE_TEXT_IN_PROTOCOL* text_in;
        EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL* text_in_ex;
    } stream;
    int status;
};

#ifdef __cplusplus
}
#endif

#endif //FILE_H
