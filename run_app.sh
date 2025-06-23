#!/bin/bash

set -e

echo "Running projectDrift"

# Read last target_arch from .last_target_arch
if [[ ! -f .last_target ]]; then
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

./build/$TARGET_ARCH/$TARGET_BOARD/deploy/projectDrift