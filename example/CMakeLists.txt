add_executable(example
        main.c
)

target_link_libraries(example
        uefi-libc
        libc++
)
