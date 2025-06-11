#!/bin/bash

# USTR Demo Runner Script
# ======================
# This script runs all demos for the USTR project

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

echo -e "${BLUE}USTR Demo Runner${NC}"
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

# Find and run all demo binaries
echo -e "${GREEN}Running USTR demos...${NC}"
echo "===================="

demo_count=0
failed_demos=0

for demo in "$BUILD_DIR"/bin/*_demo; do
    if [ -x "$demo" ]; then
        demo_name=$(basename "$demo")
        echo ""
        echo -e "${BLUE}--- Running $demo_name ---${NC}"
        
        if "$demo"; then
            echo -e "${GREEN}--- $demo_name completed successfully ---${NC}"
        else
            echo -e "${RED}--- $demo_name failed ---${NC}"
            ((failed_demos++))
        fi
        
        ((demo_count++))
    fi
done

echo ""
echo -e "${BLUE}Demo Summary:${NC}"
echo "============="

if [ $demo_count -eq 0 ]; then
    echo -e "${YELLOW}No demo binaries found in $BUILD_DIR${NC}"
    echo -e "${YELLOW}Try running the build script first: ./build.sh${NC}"
    exit 1
elif [ $failed_demos -eq 0 ]; then
    echo -e "${GREEN}✓ All $demo_count demos completed successfully!${NC}"
    exit 0
else
    echo -e "${RED}✗ $failed_demos out of $demo_count demos failed${NC}"
    exit 1
fi
