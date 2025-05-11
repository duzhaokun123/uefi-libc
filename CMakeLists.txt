cmake_minimum_required(VERSION 3.31)

if (NOT CMAKE_SYSTEM_PROCESSOR)
    message(FATAL_ERROR "CMAKE_SYSTEM_PROCESSOR is not set, please set it to x86_64, aarch64 or riscv64")
endif ()

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
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

install(DIRECTORY cmake/Platform
        DESTINATION share/cmake/Modules
        FILES_MATCHING
        PATTERN "*.cmake"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
)

configure_file(
        uef-clang.cmake.in "uefi-clang.cmake" @ONLY
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/uefi-clang.cmake"
        DESTINATION share/cmake/toolchain
        RENAME "${CMAKE_SYSTEM_PROCESSOR}-uefi-clang.cmake"
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ
)
