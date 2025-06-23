cmake_minimum_required(VERSION 3.16)
project(projectDrift)

# # Enable sanitizers if requested
# option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
# option(ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer" OFF)
# option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)

# if (ENABLE_ASAN)
#     message(STATUS "AddressSanitizer enabled")
#     set(SANITIZER_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
# elseif (ENABLE_UBSAN)
#     message(STATUS "UndefinedBehaviorSanitizer enabled")
#     set(SANITIZER_FLAGS "-fsanitize=undefined -fno-omit-frame-pointer")
# elseif (ENABLE_TSAN)
#     message(STATUS "ThreadSanitizer enabled")
#     set(SANITIZER_FLAGS "-fsanitize=thread -fno-omit-frame-pointer")
# endif()

if(SANITIZER_FLAGS)
    add_compile_options(${SANITIZER_FLAGS})
    add_link_options(${SANITIZER_FLAGS})
endif()

set(CMAKE_CXX_STANDARD 17)
message(STATUS "pybind11_add_module available!")

# if(IS_PI_TOOLCHAIN)
#     message(STATUS "NOW USING RPI TOOLCHAIN")
#     # Find Python for cross-compilation
#     find_package(Python3 REQUIRED COMPONENTS Development)
# endif()

message(STATUS "Python3 version: ${Python3_VERSION}")
message(STATUS "Python3 executable: ${Python3_EXECUTABLE}")
message(STATUS "Python3 include dirs: ${Python3_INCLUDE_DIRS}")
message(STATUS "Python3 libraries: ${Python3_LIBRARIES}")

# Set output directory for all executable targets
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/deploy)

# Read contents of .last_board
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/.last_target_board" TARGET_BOARD_RAW)
# Strip trailing whitespace/newlines
string(STRIP "${TARGET_BOARD_RAW}" TARGET_BOARD)

# Read contents of .last_board
file(READ "${CMAKE_CURRENT_SOURCE_DIR}/.last_target_arch" TARGET_ARCH_RAW)
# Strip trailing whitespace/newlines
string(STRIP "${TARGET_ARCH_RAW}" TARGET_ARCH)

add_subdirectory(src)
add_subdirectory(external)
add_subdirectory(python_wrappers)


