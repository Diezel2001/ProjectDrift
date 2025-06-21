#!/bin/bash

set -e

echo "Running projectDrift"

# Read last target from .last_target
if [[ ! -f .last_target ]]; then
    echo "Error: .last_target file not found. Run ./setup.sh first."
    exit 1
fi

TARGET=$(<.last_target)

./build/$TARGET/deploy/projectDrift