option(UEFI_GNU_EFI_DIR "Path to gnu-efi" "${CMAKE_CURRENT_SOURCE_DIR}/gnu-efi")

if (UEFI_GNU_EFI_DIR STREQUAL "OFF")
    set(UEFI_GNU_EFI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gnu-efi")
    message(STATUS "using default gnu-efi path: ${UEFI_GNU_EFI_DIR}")
endif ()

aux_source_directory(src SRC_FILES)

add_library(uefi-libc STATIC
        ${SRC_FILES}
)

target_include_directories(uefi-libc PUBLIC
        include
        ${UEFI_GNU_EFI_DIR}/inc
        ${UEFI_GNU_EFI_DIR}/inc/protocol
        ${UEFI_GNU_EFI_DIR}/inc/${CMAKE_SYSTEM_PROCESSOR}
)

target_link_libraries(uefi-libc PUBLIC
    libc++
)
