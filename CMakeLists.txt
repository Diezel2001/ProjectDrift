cmake_minimum_required(VERSION 3.16)
project(projectDrift)

set(CMAKE_CXX_STANDARD 17)
message(STATUS "pybind11_add_module available!")

# Set output directory for all executable targets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/deploy)

add_subdirectory(src)
add_subdirectory(external)
add_subdirectory(python_wrappers)


