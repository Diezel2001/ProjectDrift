set(IS_PI_TOOLCHAIN TRUE)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

# Cross-compile for Raspberry Pi 4 (32-bit Raspberry Pi OS)
set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

set(SYSROOT ${CMAKE_CURRENT_LIST_DIR}/raspberrypi-sysroot)
set(CMAKE_SYSROOT ${SYSROOT})
set(CMAKE_FIND_ROOT_PATH ${SYSROOT})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Set Python variables EARLY so CMake can pick them up
set(Python3_ROOT_DIR ${SYSROOT}/usr CACHE PATH "" FORCE)
set(Python3_INCLUDE_DIR ${SYSROOT}/usr/include/python3.9 CACHE PATH "" FORCE)
set(Python3_LIBRARY ${SYSROOT}/usr/lib/arm-linux-gnueabihf/libpython3.9.so CACHE FILEPATH "" FORCE)
set(Python3_EXECUTABLE /usr/bin/python3 CACHE FILEPATH "" FORCE)  # Use HOST python executable

message(STATUS ">>> Toolchain loaded")
message(STATUS ">>> Python include: ${Python3_INCLUDE_DIR}")
message(STATUS ">>> Python lib: ${Python3_LIBRARY}")
message(STATUS ">>> Python exe: ${Python3_EXECUTABLE}")