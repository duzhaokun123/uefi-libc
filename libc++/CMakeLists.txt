aux_source_directory(src SRC_FILES)

add_library(c++ STATIC
        ${SRC_FILES}
)

target_include_directories(c++ PUBLIC
        include
)

target_link_libraries(c++ PRIVATE
        c
)

install(TARGETS c++
    ARCHIVE DESTINATION lib
)
