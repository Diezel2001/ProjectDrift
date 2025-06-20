# Cross-compile for Raspberry Pi Zero 2 W (same CPU arch as Pi 4 if 32-bit)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# Optional: Set sysroot if you have the Pi Zero 2 root filesystem
# set(CMAKE_SYSROOT /path/to/zero2/rootfs)
