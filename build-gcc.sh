#!/bin/bash
set -euo pipefail

# ------------------------------
# Configuration
# ------------------------------
BUILD_DIR=./.build
VCPKG_DIR=./vcpkg          # submodule
TRIPLET=x64-linux
CMAKE_BUILD_TYPE=Release

# ------------------------------
# Bootstrap vcpkg if needed
# ------------------------------
if [ ! -f "$VCPKG_DIR/vcpkg" ]; then
    echo "Bootstrapping vcpkg..."
    "$VCPKG_DIR/bootstrap-vcpkg.sh"
fi

# ------------------------------
# Install dependencies via vcpkg
# ------------------------------
# Example: replace with your project dependencies
"$VCPKG_DIR/vcpkg" install --triplet $TRIPLET

# ------------------------------
# Create build directory
# ------------------------------
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# ------------------------------
# Configure project with CMake
# ------------------------------
cmake -G Ninja \
    -DCMAKE_BUILD_TYPE=$CMAKE_BUILD_TYPE \
    -DCMAKE_TOOLCHAIN_FILE="$VCPKG_DIR/scripts/buildsystems/vcpkg.cmake" \
    -DCMAKE_CXX_FLAGS="-fdiagnostics-color=always" \
    ..

# ------------------------------
# Build
# ------------------------------
cmake --build .

# ------------------------------
# Run tests
# ------------------------------
ctest --output-on-failure

# ------------------------------
# Done
# ------------------------------
echo "Build completed successfully!"

