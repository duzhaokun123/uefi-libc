option(UEFI_GNU_EFI_DIR "Path to gnu-efi" "${CMAKE_CURRENT_SOURCE_DIR}/gnu-efi")

if (UEFI_GNU_EFI_DIR STREQUAL "OFF")
    set(UEFI_GNU_EFI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/gnu-efi")
    message(STATUS "using default gnu-efi path: ${UEFI_GNU_EFI_DIR}")
endif ()

file(GLOB_RECURSE SRC_FILES
#        src/{.,**}/*.{c,cpp}
        src/*.c src/**/*.c
        src/*.cpp src/**/*.cpp
)

add_library(c STATIC
        ${SRC_FILES}
)

target_compile_definitions(c PUBLIC
        UEFI_LIBC_USE_GNU_EFI
#        UEFI_LIBC_STUB_AS_TRAP
)

target_include_directories(c PUBLIC
        include
        ${UEFI_GNU_EFI_DIR}/inc
        ${UEFI_GNU_EFI_DIR}/inc/protocol
        ${UEFI_GNU_EFI_DIR}/inc/${CMAKE_SYSTEM_PROCESSOR}
)

target_include_directories(c PRIVATE
        src
)

target_sources(c PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
        FILES
        include/assert.h
#        include/complex.h
        include/ctype.h
        include/errno.h
#        include/fenv.h
#        include/float.h
#        include/inttypes.h
#        include/iso646.h
#        include/limits.h
        include/locale.h
        include/math.h
#        include/setjmp.h
#        include/signal.h
#        include/stdalign.h
        include/stdarg.h
#        include/stdatomic.h
#        include/stdbit.h
#        include/stdbool.h
#        include/stdckdint.h
#        include/stddef.h
        include/stdint.h
        include/stdio.h
        include/stdlib.h
#        include/stdmath.h
#        include/stdnoreturn.h
        include/string.h
#        include/tgmath.h
        include/time.h
#        include/uchar.h
        include/wchar.h
#        include/wctype.h

        include/unistd.h

        include/restrict.h

        include/uefi.h
)

target_link_libraries(c PRIVATE
        c++
)

if (UEFI_CLANG_FAKE_WIN32)
    target_compile_definitions(c PUBLIC
            UEFI_CLANG_FAKE_WIN32
    )
endif ()

install(TARGETS c
        ARCHIVE DESTINATION lib
        FILE_SET HEADERS
)
