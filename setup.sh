#!/bin/bash

set -e

TARGET_ARCH=$1
TARGET_BOARD=$2

if [[ -z "$TARGET_ARCH" ]]; then
    echo "Usage: ./setup.sh [posix|pi|zero]"
    exit 1
fi

if [[ -z "$TARGET_BOARD" ]]; then
    echo "Usage: ./setup.sh [posix|pi|zero] [beta|inav]"
    exit 1
fi

# Validate input
case "$TARGET_BOARD" in
    beta|inav)
        ;;
    *)
        echo "Invalid TARGET_BOARD: $TARGET_BOARD"
        echo "Valid options are: beta, inav"
        exit 1
        ;;
esac

# Validate input
case "$TARGET_ARCH" in
    posix|pi|zero)
        ;;
    *)
        echo "Invalid TARGET_ARCH: $TARGET_ARCH"
        echo "Valid options are: posix, pi, zero"
        exit 1
        ;;
esac


# Create build directories
mkdir -p build/${TARGET_ARCH}/${TARGET_BOARD}

echo "$TARGET_ARCH" > .last_target_arch
echo "$TARGET_BOARD" > .last_target_board
echo "Created build/$TARGET_ARCH/$TARGET_BOARD"