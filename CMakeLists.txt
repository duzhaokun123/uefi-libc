cmake_minimum_required(VERSION 3.31)

if (NOT CMAKE_SYSTEM_PROCESSOR)
    message(FATAL_ERROR "CMAKE_SYSTEM_PROCESSOR is not set, set it to x86_64, aarch64 or riscv64 ...")
endif ()

set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
set(CMAKE_SYSTEM_NAME "Generic")

project(uefi_libc C CXX)

option(UEFI_CLANG_FAKE_WIN32 "Use fake ${CMAKE_SYSTEM_PROCESSOR}-win32 target for clang" OFF)

if (UEFI_CLANG_FAKE_WIN32)
    set(UEFI_TARGET "${CMAKE_SYSTEM_PROCESSOR}-win32")
else ()
    set(UEFI_TARGET "${CMAKE_SYSTEM_PROCESSOR}-uefi")
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 20.1.3)
        message(WARNING "Clang version is less than 20.1.3, enable UEFI_CLANG_FAKE_WIN32, or code may not compile")
    endif ()
endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)

set(COMMON_FLAGS " -fno-stack-check -mno-red-zone -fno-rtti")
if (UEFI_CLANG_FAKE_WIN32)
    string(APPEND COMMON_FLAGS " ")
endif ()
string(APPEND CMAKE_C_FLAGS " ${COMMON_FLAGS}")
string(APPEND CMAKE_CXX_FLAGS " ${COMMON_FLAGS} -fno-exceptions")

set(CMAKE_C_COMPILER_TARGET ${UEFI_TARGET})
set(CMAKE_CXX_COMPILER_TARGET ${UEFI_TARGET})

add_subdirectory(libc)
add_subdirectory(libc++)

install(DIRECTORY cmake/Platform
        DESTINATION share/cmake/Modules
        FILES_MATCHING
        PATTERN "*.cmake"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
)

configure_file(
        uef-clang.cmake.in uefi-clang.cmake @ONLY
)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/uefi-clang.cmake"
        DESTINATION share/cmake/toolchain
        RENAME "${CMAKE_SYSTEM_PROCESSOR}-uefi-clang.cmake"
        PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ
)


configure_file(
        cmake/Platform/UEFI-Clang.cmake.in cmake/Platform/UEFI-Clang.cmake @ONLY
)

install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/cmake/Platform"
        DESTINATION share/cmake/Modules
        FILES_MATCHING
        PATTERN "*.cmake"
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
)