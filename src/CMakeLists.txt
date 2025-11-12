# Find the directory on the machine that contains the libheif header file (libheif/heif.h).
# Search in the given filepath. On my machine, this is /usr/include/x86_64-linux-gnu.
# Save the path of the directory path to the LIBHEIF_INCLUDE_DIR variable.
find_path(LIBHEIF_INCLUDE_DIR
    NAMES libheif/heif.h
    PATHS /usr/include/x86_64-linux-gnu
)

# Find the library object code file on the machine (libheif.so).
# Search in the given path for the name (lib is appended to the start of the name).
# On my machine, the path is /usr/include/x86_64-linux-gnu.
# Save the path of the library to the LIBHEIF_LIBRARY variable.
find_library(LIBHEIF_LIBRARY 
    NAMES heif libheif
    PATHS /usr/lib/x86_64-linux-gnu
)

# Repeat above with libjpeg
find_path(LIBJPEG_INCLUDE_DIR
    NAMES jpeglib.h
    PATHS /usr/include/x86_64-linux-gnu
)

find_library(LIBJPEG_LIBRARY
    NAMES jpeg libjpeg
    PATHS /usr/lib/x86_64-linux-gnu
)


# Create an executable named noHEIC
add_executable(noHEIC main.c)

# Allow the program to access libheif and libjpeg headers
# Private since only used in internal implementation
target_include_directories(noHEIC PRIVATE 
    ${LIBHEIF_INCLUDE_DIR}
    ${LIBJPEG_INCLUDE_DIR}
)

# Instruct the linker to link the libheif and libjpeg library to the program
# Private since only used in internal implementation
target_link_libraries(noHEIC PRIVATE 
    ${LIBHEIF_LIBRARY}
    ${LIBJPEG_LIBRARY}    
)

