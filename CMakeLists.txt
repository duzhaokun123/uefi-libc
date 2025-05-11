cmake_minimum_required(VERSION 3.31)

if (NOT CMAKE_SYSTEM_PROCESSOR)
    message(FATAL_ERROR "CMAKE_SYSTEM_PROCESSOR is not set, please set it to x86_64, aarch64 or riscv64")
endif ()

set(CMAKE_SYSTEM_NAME "UEFI")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(uefi_libc C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

set(COMMON_FLAGS " -fno-stack-check -mno-red-zone -fno-rtti")
string(APPEND CMAKE_C_FLAGS " ${COMMON_FLAGS}")
string(APPEND CMAKE_CXX_FLAGS " ${COMMON_FLAGS} -fno-exceptions")

add_subdirectory(libc)
add_subdirectory(libc++)
