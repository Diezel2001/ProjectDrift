## Enable QEMU emulation:

    docker run --rm --privileged multiarch/qemu-user-static --reset -p yes

## Build with platform:

    docker build --platform=linux/arm64 -t rpi-aarch64-builder .

## Run container:

    docker run --rm -it --platform=linux/arm64 -v "$PWD:/project" rpi-aarch64-builder

This will emulate Raspberry Pi's 64-bit Debian environment inside Docker on your x86 PC.