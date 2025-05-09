aux_source_directory(src SRC_FILES)

add_library(libc++ STATIC
        ${SRC_FILES}
)

target_include_directories(libc++ PUBLIC
        include
)

target_link_libraries(libc++ PUBLIC
        uefi-libc
)