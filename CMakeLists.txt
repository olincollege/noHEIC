# Minimum required CMake version
cmake_minimum_required(VERSION 3.22)

# Define the project name, version, and langauges
project(noHEIC
  VERSION 1.0
  LANGUAGES C
)

# Add more CMake instructions in a subdirectory
add_subdirectory(src)