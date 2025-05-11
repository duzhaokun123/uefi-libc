option(UEFI_GNU_EFI_DIR "Path to gnu-efi" "${CMAKE_CURRENT_SOURCE_DIR}/gnu-efi")

if (UEFI_GNU_EFI_DIR STREQUAL "OFF")
    set(UEFI_GNU_EFI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gnu-efi")
    message(STATUS "using default gnu-efi path: ${UEFI_GNU_EFI_DIR}")
endif ()

aux_source_directory(src SRC_FILES)

add_library(c STATIC
        ${SRC_FILES}
)

target_include_directories(c PUBLIC
        include
        ${UEFI_GNU_EFI_DIR}/inc
        ${UEFI_GNU_EFI_DIR}/inc/protocol
        ${UEFI_GNU_EFI_DIR}/inc/${CMAKE_SYSTEM_PROCESSOR}
)

target_sources(c PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
        include/assert.h
        include/errno.h
        include/restrict.h
        include/stdarg.h
        include/stdio.h
        include/stdlib.h
        include/string.h
        include/wchar.h
        include/uefi.h
)

target_link_libraries(c PRIVATE
        c++
)

install(TARGETS c
        ARCHIVE DESTINATION lib
        FILE_SET HEADERS
)
