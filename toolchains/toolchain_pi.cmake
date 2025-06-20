# Cross-compile for Raspberry Pi 4 (32-bit Raspberry Pi OS)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

# Optional: If using sysroot (Raspberry Pi root filesystem)
# set(CMAKE_SYSROOT /path/to/rpi4/rootfs)