# Cross-compile for Raspberry Pi (64-bit)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

set(CMAKE_C_COMPILER /home/diether/src/raspberrypi/sysroot/usr/lib/gcc/aarch64-linux-gnu)
set(CMAKE_CXX_COMPILER /home/diether/src/raspberrypi/sysroot/usr/bin/aarch64-linux-gnu-g++)

# Use your extracted sysroot
set(SYSROOT /home/diether/src/raspberrypi/sysroot)  # <-- change to your actual path
set(CMAKE_SYSROOT ${SYSROOT})
set(CMAKE_FIND_ROOT_PATH ${SYSROOT})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

# Python config (adjust version)
set(PYTHON_VERSION 3.11)
set(Python3_INCLUDE_DIR ${SYSROOT}/usr/include/python${PYTHON_VERSION})
set(Python3_LIBRARY ${SYSROOT}/usr/lib/aarch64-linux-gnu/libpython${PYTHON_VERSION}.so)


