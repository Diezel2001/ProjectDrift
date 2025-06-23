#!/bin/bash

set -e

# Read last target_arch from .last_target_arch
if [[ ! -f .last_target_arch ]]; then
    echo "Error: .last_target_arch file not found. Run ./setup.sh first."
    exit 1
fi

# Read last target_board from .last_target_board
if [[ ! -f .last_target_board ]]; then
    echo "Error: .last_target_board file not found. Run ./setup.sh first."
    exit 1
fi

TARGET_ARCH=$(<.last_target_arch)
TARGET_BOARD=$(<.last_target_board)

# Optional second argument: make target (e.g., --clean)
MAKE_TARGET=$1

TOOLCHAIN_FILE="toolchains/toolchain_$TARGET_ARCH.cmake"
BUILD_DIR="build/$TARGET_ARCH/$TARGET_BOARD"

# Check if toolchain file exists
if [[ ! -f "$TOOLCHAIN_FILE" ]]; then
    echo "Error: Toolchain file '$TOOLCHAIN_FILE' does not exist."
    exit 1
fi

# Check if build directory exists
if [[ ! -d "$BUILD_DIR" ]]; then
    echo "Error: Build directory '$BUILD_DIR' does not exist. Did you run setup.sh?"
    exit 1
fi

cd "$BUILD_DIR"

# Only configure with CMake if not already done
if [[ ! -f Makefile ]]; then
    echo "Running CMake configuration for target: $TARGET_ARCH-$TARGET_BOARD"
    cmake ../../../ -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE 
    # -DENABLE_ASAN=ON
fi

# Run make
if [[ -z "$MAKE_TARGET" ]]; then
    make -j$(nproc)
else
    make "${MAKE_TARGET/--/}" -j$(nproc)
fi

echo "✅ Build complete for target : $TARGET_ARCH-$TARGET_BOARD ${MAKE_TARGET:+with make target '$MAKE_TARGET'}"
