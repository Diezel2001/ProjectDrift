cmake_minimum_required(VERSION 3.16)
project(projectowl)

set(CMAKE_CXX_STANDARD 17)

# Set output directory for all executable targets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/deploy)

add_subdirectory(src)

