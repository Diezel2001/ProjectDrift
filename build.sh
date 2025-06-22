#!/bin/bash

set -e

# Read last target from .last_target
if [[ ! -f .last_target ]]; then
    echo "Error: .last_target file not found. Run ./setup.sh first."
    exit 1
fi

TARGET=$(<.last_target)

# Optional second argument: make target (e.g., --clean)
MAKE_TARGET=$1

TOOLCHAIN_FILE="toolchains/toolchain_$TARGET.cmake"
BUILD_DIR="build/$TARGET"

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
    echo "Running CMake configuration for target: $TARGET"
    cmake ../.. -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE -DENABLE_ASAN=ON
fi

# Run make
if [[ -z "$MAKE_TARGET" ]]; then
    make -j$(nproc)
else
    make "${MAKE_TARGET/--/}" -j$(nproc)
fi

echo "✅ Build complete for target: $TARGET ${MAKE_TARGET:+with make target '$MAKE_TARGET'}"
