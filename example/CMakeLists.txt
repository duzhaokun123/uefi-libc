cmake_minimum_required(VERSION 3.31)

project(uefi_libc_example C)

set(CMAKE_C_STANDARD 23)

add_executable(example
        main.c
)
