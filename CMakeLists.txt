cmake_minimum_required(VERSION 3.16)
project(projectDrift)

set(CMAKE_CXX_STANDARD 17)
message(STATUS "pybind11_add_module available!")

if(IS_PI_TOOLCHAIN)
    message(STATUS "NOW USING RPI TOOLCHAIN")
    # Find Python for cross-compilation
    find_package(Python3 REQUIRED COMPONENTS Development)
endif()

message(STATUS "Python3 version: ${Python3_VERSION}")
message(STATUS "Python3 executable: ${Python3_EXECUTABLE}")
message(STATUS "Python3 include dirs: ${Python3_INCLUDE_DIRS}")
message(STATUS "Python3 libraries: ${Python3_LIBRARIES}")

# Set output directory for all executable targets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/deploy)

add_subdirectory(src)
add_subdirectory(external)
add_subdirectory(python_wrappers)


