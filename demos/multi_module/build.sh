#!/bin/bash

# Build script for the multi-module demo

# Check if we should build standalone or use the main build
if [ -z "$1" ] || [ "$1" != "--standalone" ]; then
    echo "Building as part of the main project..."
    echo "To build standalone, use: ./build.sh --standalone"
    
    # Run the main rebuild script
    cd ../../..
    ./rebuild.sh
    
    # Run the demo if it exists
    if [ -f "./build/bin/multi_module_demo" ]; then
        echo -e "\nRunning the multi-module demo:\n"
        ./build/bin/multi_module_demo
    else
        echo "Error: multi_module_demo executable not found!"
        exit 1
    fi
    
    exit 0
fi

# Standalone build below
echo "Building standalone..."

# Clean and recreate build directory
echo "Cleaning build directory for complete rebuild..."
rm -rf build
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build
cmake --build .

# Run the demo
echo -e "\nRunning the multi-module demo:\n"
# Check if the executable is in the bin directory (if using CMake output dir)
if [ -f "bin/multi_module_demo" ]; then
    ./bin/multi_module_demo
else
    # Try the root build directory
    ./multi_module_demo
fi

# Return to the original directory
cd ..
