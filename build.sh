#!/bin/bash

# Set variables
BUILD_DIR="build"

# Create build directory if it doesn't exist
mkdir -p "$BUILD_DIR"

# Go into build directory
cd "$BUILD_DIR"

# Run cmake to generate build system
cmake ..

# Build the project using make
make

# Go back to project root
cd ..

# Optional: Run the compiled program if it exists
if [[ -f "$BUILD_DIR/src/projectowl" ]]; then
    echo -e "\n🎉 Build successful!\n"
else
    echo -e "\n❌ Build failed or binary not found."
fi

