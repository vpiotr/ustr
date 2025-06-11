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

# Check if test binary exists
TEST_BIN="$BUILD_DIR/bin/ustr_test"
if [ ! -x "$TEST_BIN" ]; then
    echo -e "${RED}Test binary not found or not executable: $TEST_BIN${NC}"
    echo -e "${YELLOW}Try running the build script first: ./build.sh${NC}"
    exit 1
fi

# Run the tests
echo -e "${GREEN}Running USTR tests...${NC}"
echo "====================="
echo ""

# Execute the test binary
"$TEST_BIN"
exit_code=$?

echo ""
if [ $exit_code -eq 0 ]; then
    echo -e "${GREEN}✓ All tests passed successfully!${NC}"
else
    echo -e "${RED}✗ Some tests failed (exit code: $exit_code)${NC}"
fi

exit $exit_code
