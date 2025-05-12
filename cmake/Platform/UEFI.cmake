set(CMAKE_EXECUTABLE_SUFFIX ".efi")
set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS FALSE)

add_link_options("LINKER:-subsystem:efi_application,-nodefaultlib,-entry:EfiMain") # no effect on uefi target
