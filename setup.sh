#!/bin/bash

set -e

TARGET=$1

if [[ -z "$TARGET" ]]; then
    echo "Usage: ./setup.sh [posix|pi|zero]"
    exit 1
fi

# Validate input
case "$TARGET" in
    posix|pi|zero)
        ;;
    *)
        echo "Invalid target: $TARGET"
        echo "Valid options are: posix, pi, zero"
        exit 1
        ;;
esac

# Create build directories
mkdir -p build/$TARGET

echo "$TARGET" > .last_target
echo "Created build/$TARGET (if not existing)"