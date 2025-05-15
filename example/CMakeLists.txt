cmake_minimum_required(VERSION 3.31)

if (NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows" AND NOT UEFI_LIBC_BUILD_EXAMPLE)
    set(CMAKE_LINKER_TYPE LLD) # always use lld on non-windows host
endif ()

project(uefi_libc_example C)

set(CMAKE_C_STANDARD 23)

if (UEFI_LIBC_BUILD_EXAMPLE)
    set(UEFI_LIBC_EXAMPLE_NAME "example.efi")
else ()
    set(UEFI_LIBC_EXAMPLE_NAME "example")
endif ()

add_executable(${UEFI_LIBC_EXAMPLE_NAME}
        main.c
)

if (UEFI_LIBC_BUILD_EXAMPLE)
    target_link_libraries(${UEFI_LIBC_EXAMPLE_NAME} PRIVATE
            c
            c++
    )
    target_link_options(${UEFI_LIBC_EXAMPLE_NAME} PRIVATE
            "-fuse-ld=lld"
            "LINKER:-subsystem:efi_application,-nodefaultlib,-entry:EfiMain"
    )
endif ()
