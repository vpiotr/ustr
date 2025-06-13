#!/bin/bash

# USTR CMake Build Script
# ======================
# This script builds the USTR project using CMake

set -e  # Exit on any error

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$SCRIPT_DIR"

echo -e "${BLUE}USTR CMake Build Script${NC}"
echo -e "${BLUE}=======================${NC}"

# Parse command line arguments
BUILD_TYPE="Release"
BUILD_TESTS="ON"
BUILD_DEMOS="ON"
BUILD_DOCS="OFF"
CLEAN_BUILD=true # Default to clean build
INSTALL_PREFIX=""
VERBOSE=false

show_help() {
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -t, --type TYPE       Build type: Debug|Release|MinSizeRel|RelWithDebInfo (default: Release)"
    echo "  --no-clean            Don't clean build directory before building"
    echo "  --no-tests            Don't build tests"
    echo "  --no-demos            Don't build demos"
    echo "  --with-docs           Build documentation (requires Doxygen)"
    echo "  --prefix PATH         Installation prefix"
    echo "  -v, --verbose         Verbose build output"
    echo "  -h, --help            Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                                    # Clean Release build with tests and demos"
    echo "  $0 -t Debug                          # Clean Debug build"
    echo "  $0 --no-clean --no-tests            # Incremental Release build without tests"
    echo "  $0 --prefix /usr/local               # Clean Release build with custom install prefix"
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -t|--type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        --no-clean)
            CLEAN_BUILD=false
            shift
            ;;
        --no-tests)
            BUILD_TESTS="OFF"
            shift
            ;;
        --no-demos)
            BUILD_DEMOS="OFF"
            shift
            ;;
        --with-docs)
            BUILD_DOCS="ON"
            shift
            ;;
        --prefix)
            INSTALL_PREFIX="$2"
            shift 2
            ;;
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            show_help
            exit 1
            ;;
    esac
done

# Validate build type
case $BUILD_TYPE in
    Debug|Release|MinSizeRel|RelWithDebInfo)
        ;;
    *)
        echo -e "${RED}Invalid build type: $BUILD_TYPE${NC}"
        echo "Valid types: Debug, Release, MinSizeRel, RelWithDebInfo"
        exit 1
        ;;
esac

# Build directory
BUILD_DIR="$PROJECT_ROOT/build"

# Clean if not explicitly disabled
if [ "$CLEAN_BUILD" = true ]; then
    echo -e "${YELLOW}Cleaning build directory for complete rebuild...${NC}"
    rm -rf "$BUILD_DIR"
fi

# Create build directory
echo -e "${YELLOW}Creating build directory...${NC}"
mkdir -p "$BUILD_DIR"
cd "$BUILD_DIR"

# Prepare CMake arguments
CMAKE_ARGS=(
    "-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
    "-DUSTR_BUILD_TESTS=$BUILD_TESTS"
    "-DUSTR_BUILD_DEMOS=$BUILD_DEMOS"
    "-DUSTR_BUILD_DOCS=$BUILD_DOCS"
)

if [ -n "$INSTALL_PREFIX" ]; then
    CMAKE_ARGS+=("-DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX")
fi

# Configure
echo -e "${GREEN}Configuring with CMake...${NC}"
echo -e "${BLUE}Build type: $BUILD_TYPE${NC}"
echo -e "${BLUE}Build tests: $BUILD_TESTS${NC}"
echo -e "${BLUE}Build demos: $BUILD_DEMOS${NC}"
echo -e "${BLUE}Build docs: $BUILD_DOCS${NC}"

if [ "$VERBOSE" = true ]; then
    echo -e "${BLUE}CMake command: cmake ${CMAKE_ARGS[*]} $PROJECT_ROOT${NC}"
fi

cmake "${CMAKE_ARGS[@]}" "$PROJECT_ROOT"

# Build
echo -e "${GREEN}Building...${NC}"
if [ "$VERBOSE" = true ]; then
    cmake --build . --verbose
else
    cmake --build .
fi

echo -e "${GREEN}Build completed successfully!${NC}"
echo ""

# Show build results
echo -e "${BLUE}Build Results:${NC}"
if [ -d "bin" ]; then
    echo -e "${BLUE}Executables in build/bin/:${NC}"
    ls -la bin/
else
    echo -e "${YELLOW}No executables built${NC}"
fi

echo ""
echo -e "${BLUE}Available targets:${NC}"
echo "  make                       - Build all"
echo "  make ustr_core_features_test - Build core features tests only"
echo "  make ustr_container_test   - Build container tests only"
echo "  make run_tests             - Build and run all tests"
if [ "$BUILD_DEMOS" = "ON" ]; then
    echo "  make ustr_demo         - Build demo only"
    echo "  make run_demos         - Build and run demos"
fi
echo "  make install       - Install the library"

echo ""
echo -e "${BLUE}Quick commands:${NC}"
if [ "$BUILD_TESTS" = "ON" ]; then
    echo -e "  Run tests:  ${GREEN}cd build && make run_tests${NC}"
    echo -e "  Run tests:  ${GREEN}cd build && ctest${NC}"
    echo -e "  Run core tests directly: ${GREEN}./build/bin/ustr_core_features_test${NC}"
    echo -e "  Run container tests directly: ${GREEN}./build/bin/ustr_container_test${NC}"
fi

if [ "$BUILD_DEMOS" = "ON" ]; then
    echo -e "  Run demos:  ${GREEN}cd build && make run_demos${NC}"
    echo -e "  Run demo directly:  ${GREEN}./build/bin/ustr_demo${NC}"
fi

echo -e "  Install:    ${GREEN}cd build && make install${NC}"

echo ""
echo -e "${GREEN}CMake build completed successfully!${NC}"
