#!/bin/bash

# USTR Test Runner Script
# ======================
# This script runs all tests for the USTR project

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
BUILD_DIR="$PROJECT_ROOT/build"

echo -e "${BLUE}USTR Test Runner${NC}"
echo -e "${BLUE}================${NC}"

# Check if build directory exists
if [ ! -d "$BUILD_DIR" ]; then
    echo -e "${YELLOW}Build directory not found. Running build script...${NC}"
    if [ -x "$PROJECT_ROOT/build.sh" ]; then
        "$PROJECT_ROOT/build.sh"
    else
        echo -e "${RED}Build script not found or not executable: $PROJECT_ROOT/build.sh${NC}"
        exit 1
    fi
fi

# Check if test binaries exist
CORE_TEST_BIN="$BUILD_DIR/bin/ustr_core_features_test"
CONTAINER_TEST_BIN="$BUILD_DIR/bin/ustr_container_test"

if [ ! -x "$CORE_TEST_BIN" ] || [ ! -x "$CONTAINER_TEST_BIN" ]; then
    echo -e "${RED}Test binaries not found or not executable:${NC}"
    [ ! -x "$CORE_TEST_BIN" ] && echo -e "${RED}- $CORE_TEST_BIN${NC}"
    [ ! -x "$CONTAINER_TEST_BIN" ] && echo -e "${RED}- $CONTAINER_TEST_BIN${NC}"
    echo -e "${YELLOW}Try running the build script first: ./build.sh${NC}"
    exit 1
fi

# Run the tests
echo -e "${GREEN}Running USTR tests...${NC}"
echo "====================="
echo ""

# Execute the test binaries
echo -e "${BLUE}Running Core Features Tests:${NC}"
"$CORE_TEST_BIN"
core_exit_code=$?

echo ""
echo -e "${BLUE}Running Container Tests:${NC}"
"$CONTAINER_TEST_BIN"
container_exit_code=$?

# Check exit codes
if [ $core_exit_code -eq 0 ] && [ $container_exit_code -eq 0 ]; then
    exit_code=0
else
    exit_code=1
fi

echo ""
if [ $exit_code -eq 0 ]; then
    echo -e "${GREEN}✓ All tests passed successfully!${NC}"
else
    echo -e "${RED}✗ Some tests failed (exit code: $exit_code)${NC}"
fi

exit $exit_code
