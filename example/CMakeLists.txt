cmake_minimum_required(VERSION 3.31)

if (NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(CMAKE_LINKER_TYPE LLD) # always use lld on non-windows host
endif ()

project(uefi_libc_example C)

set(CMAKE_C_STANDARD 23)

add_executable(example
        main.c
)
