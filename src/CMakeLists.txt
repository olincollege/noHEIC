# Find the directory on the machine that contains the libheif header file (libheif/heif.h).
# Save the path of the directory path to the LIBHEIF_INCLUDE_DIR variable.
find_path(LIBHEIF_INCLUDE_DIRS NAMES libheif/heif.h)

# Find the library object code file on the machine (libheif.so).
# Save the path of the library to the LIBHEIF_LIBRARY variable.
find_library(LIBHEIF_LIBRARY NAMES heif libheif)

# CMake includes a FINDJPEG module, making it simple to include the package
find_package(JPEG REQUIRED)

# Create an executable named noHEIC
add_executable(noHEIC main.c)

# Allow the program to access libheif headers
# Private since only used in internal implementation
target_include_directories(noHEIC PRIVATE 
    ${LIBHEIF_INCLUDE_DIRS}
)

# Instruct the linker to link the libheif and libjpeg library to the program
# Private since only used in internal implementation
target_link_libraries(noHEIC PRIVATE 
    ${LIBHEIF_LIBRARY}
    JPEG::JPEG  
)

